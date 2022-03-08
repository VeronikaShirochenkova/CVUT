#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 
void listener(int);

int main(void) {
    pid_t GEN, NSD;
    int pipefd[2];
    
    if (pipe(pipefd) == -1) {
        exit(2);
    }

    // make child process "GEN" 
    GEN = fork();
    if (GEN == -1) {
        exit(2);
    } 
    if (GEN == 0) {      
        close(pipefd[0]);               // close unused read end 
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            exit(2);
        }
        close(pipefd[1]);               // close unused write end
        signal(SIGTERM, listener);
        while (1) {
            printf("%d %d\n", rand() % 4096, rand() % 4096);
            fflush(stdout);
            sleep(1);
        }
    }

    // make child process "NSD"
    NSD = fork();
    if (NSD == -1) {
        exit(2);
    }
    if (GEN && NSD == 0) {
        close(pipefd[1]);              // close unused write end
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            exit(2);
        } 
        close(pipefd[0]);              // close unused read end
        if (execl("./nsd", "./nsd", NULL) == -1) {
            exit(2);
        }
    }

    // MAIN process
    if (GEN && NSD) {
        close(pipefd[0]);
        close(pipefd[1]);
        sleep(5);
         
        // terminate process GEN
        if (kill(GEN, SIGTERM) != 0) {
            printf("ERROR\n");
            exit(1);
        }
        
        int status1 = 0;
        int status2 = 0;
        
        if (waitpid(GEN, &status1, 0) != GEN) {
            exit(2);
        }
        if (waitpid(NSD, &status2, 0) != NSD) {
            exit(2);
        }

        int res1 = 0;
        int res2 = 0;

        if(WIFEXITED(status1)) {
            res1 = WEXITSTATUS(status1);
        }

        if (WIFEXITED(status2)) {
            res2 = WEXITSTATUS(status2);
        }
        
        // checking the final statuses of both processes
        if ( !res1 && !res2) {
            printf("OK\n");
            exit(0);
        }
        else {
            printf("ERROR\n");
            exit(1);
        }
    }
    else {
        printf("ERROR\n");
        exit(1);
    }
}

void listener(int signal) {
    write(STDERR_FILENO, "GEN TERMINATED\n", 16);
    _exit(0);
}
