#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    time_t t = time(NULL);
    struct tm *showtime = localtime(&t);
    pid_t pid;
    printf("Родительский проесс создан, его pid: %d\n", getpid());
    printf("%02d:%02d:%02d\n", showtime->tm_hour,showtime->tm_min,showtime->tm_sec);
    for(int i = 0; i < 2; i++){
        if((pid = fork()) == 0){
            printf("Дочерний процесс %d создан, его pid: %d, его ppid: %d\n", i+1, getpid(), getppid());
            printf("%02d:%02d:%02d\n", showtime->tm_hour,showtime->tm_min,showtime->tm_sec);
            exit(0);
        }
        system("ps -x");
        wait(0);        	
    }
    
    exit(0);
    return 0;
}
