#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	pid_t pid;
	printf("Процесс 1 создан, его pid: %d, его ppid: %d\n", getpid(), getppid());
	if ((pid = fork()) == 0) {
		printf("Процесс 2 создан, его pid: %d, его ppid: %d\n", getpid(), getppid());
		if ((pid = fork()) == 0) {
			printf("Процесс 3 создан, его pid: %d, его ppid: %d\n", getpid(), getppid());
			if ((pid = fork()) == 0) {
				printf("Процесс 5 создан, его pid: %d, его ppid: %d\n", getpid(), getppid());
				printf("Процесс 5 с pid %d завершил работу\n", getpid());
				exit(0);
			}
			wait(0);
			printf("Процесс 3 с pid %d завершил работу\n", getpid());
			exit(0);
		}
		wait(0);
        	if((pid=fork())==0) {
            		printf("Процесс 4 создан, его pid: %d, его ppid: %d\n", getpid(), getppid());
          		if (fork() == 0) {
				printf("Процесс 6 создан, его pid: %d, его ppid: %d\n", getpid(), getppid());
                		if ((pid = fork()) == 0) {
				    	printf("Процесс 7 создан, его pid: %d, его ppid: %d\n", getpid(), getppid());
				    	execl("/bin/whoami", "whoami", NULL);
				    	printf("Процесс 7 с pid %d завершил работу\n", getpid());
				    	exit(0);
	    			}
                		wait(0);
				printf("Процесс 6 с pid %d завершил работу\n", getpid());
				exit(0);
				}
			wait(0);
			printf("Процесс 4 с pid %d завершил работу\n", getpid());
			exit(0);
      		}
		wait(0);
		printf("Процесс 2 с pid %d завершил работу\n", getpid());
		exit(0);
	}
	wait(0);
	printf("Процесс 1 с pid %d завершил работу\n", getpid());
	exit(0);
	return 0;
	

}
