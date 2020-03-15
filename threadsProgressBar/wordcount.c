#include "wordcount.h"
#include <pthread.h>
#include <wctype.h>
#include <wchar.h>

// Returns wordcount with the file and file bytes as input
long wordcount(FILE* input, long total_bytes)
{
    // Allocate Memory for the Progress Status
    struct PROGRESS_STATUS* status = malloc(sizeof(*status));

    // Initialize member variables
    long curr_bytes = 0;
    status->CurrentStatus = &curr_bytes;
    status->InitialValue = 0;
    status->TerminationValue = total_bytes;

    // Create Thread
    pthread_t writer;
    pthread_create(&writer, NULL, (void*)progress_monitor, status);

    wint_t wide_c;
    long curr_words = 0;
    int seq_len = 0;

    // Loop through file one character at a time.
    do {
        wide_c = fgetwc(input);

        // End of file, break out of loop
        if (feof(input)) {
            break;
        }

        // Check for white space and word completeion
        if (iswspace(wide_c)) {
            if (seq_len > 0) {
                curr_words++;
            }
            seq_len = 0;
        }
        else {
            seq_len++;
        }

        curr_bytes++;
    } while (1);

    pthread_join(writer, NULL);

    return curr_words;
}

void* progress_monitor(PROGRESS_STATUS* status) {
    char bar[40];
    int num_hyphens = 0;
    int curr_hyphens = 0;
    int i;
    double percent;
    while (1) {
        percent = (double)(*status->CurrentStatus) / (double)(status->TerminationValue);
        num_hyphens = (int)(percent * 40);
        for (i = 0; i < num_hyphens; i++) {
            bar[i] = '-';
        }

        // Update the bar if the num_hyphens updated after the last loop.
        if (curr_hyphens < num_hyphens) {
            printf("\r%s", bar);
            fflush(stdout);
            curr_hyphens = num_hyphens;
        }

        // Current Status = Termination Value
        if (percent == 1) {
            break;
        }
    }
}