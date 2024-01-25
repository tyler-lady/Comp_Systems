#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFER_SIZE 4096 // had to increase buffer size dramatically to handle all medium and long words

// searches file at filename for word
void searchWordInFile(const char *filename, const char *word) {
    FILE *file = fopen(filename, "r");
    //Did opening file fail?
    if (file == NULL) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    //create buffer for reading
    char buffer[BUFFER_SIZE];
    int word_count = 0;
    size_t word_len = strlen(word);

    //nested while loops could be messy
    //this fgets() looping thru the file and filling buffer until the filled value is null
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        char *pos = buffer; //copy buffer to search thru it for substrings matching our word
        while ((pos = strstr(pos, word)) != NULL) {
            word_count++;
            pos += word_len;
        }
    }

    printf("%s - %d\n", filename, word_count); //print the results of our child process
    fclose(file); //close file once finished w/ it for cleanup
}

int main(int argc, char *argv[]) {
    //require at least two arguments so that a potential file name is input
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file1 file2 ... fileN\n", argv[0]);
        return EXIT_FAILURE;
    }

    char word[BUFFER_SIZE]; //init word to be searched
    printf("Enter a word to search: "); //prompt user and store word
    scanf("%s", word);

    // was having issues with not call wait() enough, though my searches were successful
    int num_processes = argc - 1; // Number of child processes to create

    // Create new process for each file
    for (int i = 1; i < argc; i++) {
        pid_t cpid = fork();
        //handle error
        if (cpid < 0) {
            perror("Fork failed");
            return EXIT_FAILURE;
        }
        //child processes operation
        if (cpid == 0) { // Child process
            searchWordInFile(argv[i], word);
            return EXIT_SUCCESS;
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
