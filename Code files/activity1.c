#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    int pid, ret_exec, status;
    pid = fork();

    switch (pid)
    {

    case -1:
        perror("Error"); // Display error message
        exit(1);         // Exit from the program

    case 0:
        ret_exec = execl("/bin/ps", "ps", "-A", NULL);
        if (ret_exec == -1)
        {
            perror("Error executing exec"); // Display error message
            exit(1);
        }
        break;

    default:
        pid = wait(&status); // Wait for termination of the child process
        if (WIFEXITED(status))
        {
            printf("Parent process ID: %d\n", getpid());       // Process ID of the parent process
            printf("Child process ID: %d\n", pid);             // Process ID of the child process
            printf("Termination status of child: %d\n", WEXITSTATUS(status)); // Termination status of the child process
        }
        break;
    }

    return 0;
}
