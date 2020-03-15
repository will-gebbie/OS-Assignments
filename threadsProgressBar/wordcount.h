#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct PROGRESS_STATUS{
    long* CurrentStatus;
    long InitialValue;
    long TerminationValue;
} PROGRESS_STATUS;

long wordcount(FILE *input, long total_bytes);

void *progress_monitor(PROGRESS_STATUS *status);