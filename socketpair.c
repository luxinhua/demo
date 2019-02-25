#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>

#define SOCKET_BUFFER_SIZE 500

void *function_thread(void* arg){
	int fd = (int)arg;
	char buf[500];
	int len;
	int cnt = 0;
	while(1){
		// 2, thread1 send  hello to main
		len = sprintf(buf," [hello main ,cnt = %d] \n",cnt++);
		write(fd,buf,len);

		// 3, read data form main 
		len = read(fd, buf,500);
		buf[len] = '\0';
		printf("thread1 get data %s \n",buf);
		sleep(4);
	}
	return ;
}

int main(int argc,char ** argv){
	char buf[500];
	int len;
	int cnt = 0;
	int sockets[2];
	int bufferSize = SOCKET_BUFFER_SIZE;

	socketpair(AF_UNIX,SOCK_SEQPACKET,0,sockets);
	setsockopt(sockets[0],SOL_SOCKET,SO_SNDBUF,&bufferSize,sizeof(bufferSize));
	setsockopt(sockets[0],SOL_SOCKET,SO_RCVBUF,&bufferSize,sizeof(bufferSize));
	setsockopt(sockets[1],SOL_SOCKET,SO_SNDBUF,&bufferSize,sizeof(bufferSize));
	setsockopt(sockets[1],SOL_SOCKET,SO_RCVBUF,&bufferSize,sizeof(bufferSize));
	
	// 1, create thread1 
	pthread_t threadID;
	pthread_create(&threadID,NULL,&function_thread,(void*)sockets[1]);
	
	while(1){
		// 1, read data first ,   
		len = read(sockets[0], buf,500);
		buf[len] = '\0';
		printf("main get data %s \n",buf);
		// 2, thread1 send  hello to main
		len = sprintf(buf," [hello , thread ,cnt = %d]\n",cnt++);
		write(sockets[0],buf,len);
	}

	return 0;
}
