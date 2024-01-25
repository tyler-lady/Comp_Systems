#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 4096 // had to increase buffer size dramatically to handle all medium and long words

typedef struct {
    const char *filename;
    const char *word;
    int start;
    int end;
    int word_count;
} ThreadData;

// searches file at filename for word
void *searchWordInFile(void *data) {
    ThreadData *threadData = (ThreadData *)data;
    FILE *file = fopen(threadData->filename, "r");
    if (file == NULL) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    // Adjust file position to the start of the segment
    fseek(file, threadData->start, SEEK_SET);

    char buffer[BUFFER_SIZE];
    int word_count = 0;
    size_t word_len = strlen(threadData->word);
    int segment_size = threadData->end - threadData->start;

    while (ftell(file) < threadData->end) {
        int read_size = BUFFER_SIZE < segment_size ? BUFFER_SIZE : segment_size;
        int bytes_read = fread(buffer, 1, read_size, file);
        if (bytes_read <= 0) {
            break;
        }

        char *pos = buffer;
        while ((pos = strstr(pos, threadData->word)) != NULL) {
            word_count++;
            pos += word_len;
        }
        segment_size -= bytes_read;
    }

    threadData->word_count = word_count;
    fclose(file);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    //require at least two arguments so that a potential file name is input
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <number_of_threads> file1 file2 ... fileN\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "Number of threads should be greater than 0.\n");
        return EXIT_FAILURE;
    }

    char word[BUFFER_SIZE]; //init word to be searched
    printf("Enter a word to search: "); //prompt user and store word
    scanf("%s", word);

    // was having issues with not call wait() enough, though my searches were successful
    int num_processes = argc - 1; // Number of child processes to create

    // Create new process for each file
    for (int i = 2; i < argc; i++) {
        pid_t cpid = fork();
        //handle error
        if (cpid < 0) {
            perror("Fork failed");
            return EXIT_FAILURE;
        }
        //child processes operation
        if (cpid == 0) { // Child process

            pthread_t threads[num_threads];
            ThreadData thread_data[num_threads];
            FILE *file = fopen(argv[i], "r");
            if (file == NULL) {
                perror("File opening failed");
                return EXIT_FAILURE;
            }

            fseek(file, 0, SEEK_END);
            int file_size = ftell(file);
            int segment_size = file_size / num_threads;

            for (int j = 0; j < num_threads; j++) {
                thread_data[j].filename = argv[i];
                thread_data[j].word = word;
                thread_data[j].start = j * segment_size;
                thread_data[j].end = (j == num_threads - 1) ? file_size : (j + 1) * segment_size;

                pthread_create(&threads[j], NULL, searchWordInFile, (void *)&thread_data[j]);
            }

            for (int j = 0; j < num_threads; j++) {
                pthread_join(threads[j], NULL);
            }

            int total_word_count = 0;
            for (int j = 0; j < num_threads; j++) {
                total_word_count += thread_data[j].word_count;
            }

            printf("Occurrences of '%s' in %s: %d\n", word, argv[i], total_word_count);
            fclose(file);
        }

    }
    // Wait for all child processes to finish
    int status;
    pid_t wpid;
    //changed from while loop to for loop to avoid missing any wait() calls
    for (int i = 0; i < num_processes; i++) {
        wpid = wait(&status);
        //handle error
        if (wpid == -1) {
            perror("Wait failed");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
