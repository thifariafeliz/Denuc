#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

#include "../inc/logs.h"
#include "../inc/list.h"
#include "../inc/queue.h"
#include "../inc/errors.h"
#include "../inc/denuncia.h"
#include "../inc/file.h"
#include "../inc/stringo.h"
#include "../inc/utils.h"

#define DATA_FILE "denuncias.txt"
#define LOG_FILE "denuc.log"

static void denuncia_destroy(void *data) {
    if (data == NULL) {
        return;
    }
    Denuncia *denuncia = (Denuncia*)data;
    if (denuncia->title != NULL) {
        free(denuncia->title->data);
        free(denuncia->title);
    }
    free(denuncia);
}

static void print_menu(void) {
    printf("\n=== Denuc - Report Management System ===\n");
    printf("1. Print reports\n");
    printf("2. Add a report\n");
    printf("3. Complete the first report in queue\n");
    printf("4. Quit\n");
    printf("\nEnter your choice: ");
}

static void print_reports(Queue *queue) {
    if (queue == NULL || queue_size(queue) == 0) {
        printf("\nNo reports available.\n");
        return;
    }

    printf("\n=== Reports ===\n");
    Node *node = queue->head;
    int count = 1;

    while (node != NULL) {
        Denuncia *denuncia = (Denuncia*)node->data;
        const char *status_str = "Unknown";

        switch (denuncia->status) {
            case 0:
                status_str = "Pending";
                break;
            case 1:
                status_str = "In Progress";
                break;
            case 2:
                status_str = "Done";
                break;
        }

        printf("[%d] ID: %d | Title: %s | Status: %s\n",
               count, denuncia->id, denuncia->title->data, status_str);
        node = node->next;
        count++;
    }

    printf("\nTotal: %d report(s)\n", queue_size(queue));
}

static void add_report(Queue *queue, FILE *logfile) {
    printf("\n=== Add New Report ===\n");

    printf("Enter report title: ");
    Stringo *title = stringo_create("");
    if (title == NULL || title->data == NULL) {
        LogInfo loginfo = {.level = LOG_ERROR, .message = "Failed to create string for title.", .detail = "stringo_create returned NULL."};
        log_to_file(logfile, loginfo);
        printf("Error: Failed to allocate memory for title.\n");
        return;
    }

    Stringo_Error str_err = stringo_take_input(title);
    if (str_err != STRINGO_OK || title->length == 0) {
        printf("Error: Invalid title.\n");
        free(title);
        return;
    }

    printf("Enter report ID (integer): ");
    char id_buffer[64];
    if (fgets(id_buffer, sizeof(id_buffer), stdin) == NULL) {
        printf("Error: Failed to read ID.\n");
        free(title);
        return;
    }

    IntParseResult id_result = parse_int(id_buffer);
    if (id_result.status != PARSE_OK) {
        printf("Error: Invalid ID format.\n");
        free(title);
        return;
    }

    Denuncia *denuncia = malloc(sizeof(Denuncia));
    if (denuncia == NULL) {
        LogInfo loginfo = {.level = LOG_FATAL, .message = "Failed to allocate report.", .detail = "malloc returned NULL."};
        log_to_file(logfile, loginfo);
        printf("Error: Failed to allocate memory for report.\n");
        free(title);
        return;
    }

    denuncia->id = id_result.value;
    denuncia->status = 0; // Pending
    denuncia->title = title;

    List_Error err = queue_enqueue(queue, denuncia);
    if (err != LIST_OK) {
        LogInfo loginfo = {.level = LOG_ERROR, .message = "Failed to enqueue report.", .detail = "queue_enqueue returned error."};
        log_to_file(logfile, loginfo);
        printf("Error: Failed to add report to queue.\n");
        free(title);
        free(denuncia);
        return;
    }

    printf("Report added successfully! (ID: %d, Title: %s)\n", denuncia->id, denuncia->title->data);
}

static void complete_first_report(Queue *queue, FILE *logfile) {
    if (queue == NULL || queue_size(queue) == 0) {
        printf("\nNo reports in queue to complete.\n");
        return;
    }

    Denuncia *denuncia = NULL;
    List_Error err = queue_dequeue(queue, (void**)&denuncia);

    if (err != LIST_OK || denuncia == NULL) {
        LogInfo loginfo = {.level = LOG_ERROR, .message = "Failed to dequeue report.", .detail = "queue_dequeue returned error."};
        log_to_file(logfile, loginfo);
        printf("Error: Failed to complete report.\n");
        return;
    }

    printf("\nCompleted report: ID: %d | Title: %s\n", denuncia->id, denuncia->title->data);

    free(denuncia->title->data);
    free(denuncia->title);
    free(denuncia);
}

int main(void) {
    FILE *logfile = fopen(LOG_FILE, "a");
    if (logfile == NULL) {
        fprintf(stderr, "Warning: Could not open log file %s\n", LOG_FILE);
    }

    Queue queue;
    List_Error init_err = list_init(&queue, denuncia_destroy);
    if (init_err != LIST_OK) {
        fprintf(stderr, "Error: Failed to initialize queue.\n");
        if (logfile) fclose(logfile);
        return EXIT_FAILURE;
    }

    FILE *datafile = fopen(DATA_FILE, "r");
    if (datafile != NULL) {
        LogInfo loginfo = {0};
        File_Error file_err = append_to_queue(datafile, &queue, &loginfo);
        if (file_err != FILE_OK) {
            if (logfile) {
                log_to_file(logfile, loginfo);
            }
            fprintf(stderr, "Warning: Error reading data file.\n");
        }
        fclose(datafile);
    }

    int choice;
    int running = 1;

    printf("\nWelcome to Denuc - Report Management System\n");

    while (running) {
        print_menu();

        int scan = scanf("%d", &choice);
        if (scan != 1 || scan == EOF) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        switch (choice) {
            case 1:
                print_reports(&queue);
                break;
            case 2:
                add_report(&queue, logfile);
                break;
            case 3:
                complete_first_report(&queue, logfile);
                break;
            case 4:
                running = 0;
                printf("\nSaving reports to file...\n");

                datafile = fopen(DATA_FILE, "w");
                if (datafile != NULL) {
                    LogInfo loginfo = {0};
                    File_Error file_err = write_to_file(datafile, &queue, &loginfo);
                    if (file_err != FILE_OK && logfile) {
                        log_to_file(logfile, loginfo);
                    }
                    fclose(datafile);
                    printf("Reports saved to %s\n", DATA_FILE);
                } else {
                    printf("Warning: Could not save reports to file.\n");
                }

                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter 1-4.\n");
        }
    }

    list_destroy(&queue);

    if (logfile) {
        fclose(logfile);
    }

    return EXIT_SUCCESS;
}
