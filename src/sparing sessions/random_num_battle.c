#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

int create_child_process_get_exit_status() {
    pid_t pid;
    int status;
    
    pid = fork();
    
    if (pid < 0) {
        printf("Failed to create child process");
    }
    
    else if (pid == 0) {
        srand(time(NULL) ^ getpid());
        exit(rand() % 256);
    }
    
    else {
        wait(&status);
        if(WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        
        else return -1;
    }
}

int main() {
    pid_t pid;
    int status, r1, r2;
    
    r1 = create_child_process_get_exit_status();
    r2 = create_child_process_get_exit_status();

    if (r1 > r2) {
        printf("First Child Process won! %d > %d\n", r1, r2);
    }
    
    else if (r1 < r2) {
        printf("Second Child Process won! %d > %d\n", r2, r1);
    }

    else {
        printf("It is a tie!! %d = %d\n", r1, r2);
    }

    return 0;
}
