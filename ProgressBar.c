#include <stdio.h>  
#include <string.h>  
#include <unistd.h>
#include <signal.h> 
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

void
proc(pid_t pid){
	int i , j ;
	char fh[] = {'-','-','\\','\\','|','|','/','/'};
	while(1){
		for (j = 1; j<= 50; ++j){
			printf("\rprograss:[ ");
			for (i = 1 ; i <= 50 ; ++i){
				if (i  == j){
					printf("\033[34m#\033[0m");
				}else{
					printf("\033[33m-\033[0m");
				}
			}
			printf(" ] [ \033[34m %c runing...\033[0m ]",fh[j%8]); 
			fflush(stdout);
			usleep(50*1000);
			int status;
			if(waitpid(pid, &status, WNOHANG) == pid){
				printf("\r进度：[ "); 
				for (i = 1; i <= 50; ++i){ 
					printf("\033[34m#\033[0m");  
				}		
				if(errno != EINTR){
					printf(" ] [ \033[32m    done    \033[0m ]");
				}else{
					printf(" ] [ \033[32m    done    \033[0m ]");
				}
				fflush(stdout);
				return;
			}
		}
	}
}

void 
start_proc(char* tip,pid_t pid){
	printf("%s   pid:%d",tip,pid);
	proc(pid);
	printf("\n");
}

int 
main(){
	printf("\033[34m#\033[0m");
	pid_t pid;
	pid = fork();
	if (pid < 0) {
		printf("Process GET Error \n");
	}else if (pid == 0){
		int i = 0;
		sleep(10);
		return 0;
	}else{
		start_proc("start ENV ...",pid);
	}

	return 0;
}
