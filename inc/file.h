#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include "logs.h"
#include "queue.h"
#include "errors.h"

File_Error append_to_queue(FILE *file, Queue *queue, LogInfo *loginfo);
File_Error write_to_file(FILE *file, Queue *queue, LogInfo *loginfo);

#endif
