#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

int main()
{
    int pipe_fd[2]; // File descriptors for the pipe
    char command[BUFFER_SIZE]; // Buffer to hold the command
    pid_t child_pid;

    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Fork into parent and child processes
        child_pid = fork();

        if (child_pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) { // Child process
            close(pipe_fd[1]); // Close write end of the pipe

            // Read the command from the pipe sent by the parent
            read(pipe_fd[0], command, BUFFER_SIZE);

            char path[100] = "";

            if (strcmp(command, "id") == 0)
            {
                // do something
                strcat(path, "/bin/id");
            }
            else if (strcmp(command, "pwd") == 0)
            {
                // do something else
                strcat(path, "/bin/pwd");
            }
                /* more else if clauses */
            else if (strcmp(command, "date") == 0)
            {
                strcat(path, "/bin/date");
            }

            // Execute the command using execl
            if (execl(path, command, NULL) == -1) {
                perror("Exec failed");
                exit(EXIT_FAILURE);
            }
        } else { // Parent process
            close(pipe_fd[0]); // Close read end of the pipe

            printf("prompt$ ");
            scanf("%s", command);

            // Write the command to the pipe for the child to read
            write(pipe_fd[1], command, BUFFER_SIZE);

            // Wait for the child process to complete
            wait(NULL);

            // Print confirmation of the completion of the command
            printf("Done - %s\n", command);

            break;
        }
    }

    return 0;
}
