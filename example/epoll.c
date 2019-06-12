#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int add_to_epoll(int epollfd, int tmpfd){
	int result;
	struct epoll_event eventItem;
	memset(&eventItem,0,sizeof(eventItem));
	eventItem.events = EPOLLIN;
	eventItem.data.fd = tmpfd;
	    
	if( (result = epoll_ctl(epollfd,EPOLL_CTL_ADD,tmpfd,&eventItem)) == -1){
	   	printf("epoll add  result:%d |  Error:%d\n",result,errno);
	}
	printf("add finish \n");
	return result;
}


int main(int argc,char ** argv){
	struct epoll_event mPendingEventItems[16];
	char buf[256];
	int mEpollFd;
	int argvFd;
	int i;
	int epoll_result;
	int result;
	
	if (argc<2){
		printf("Usage: xxx <dir1> [dir2] [dir3] ...\n");
		return 0;
	}

	/*
	 * epoll create 
	 */
	mEpollFd = epoll_create(16);
	printf("create\n");
	/*
	 * for each file 
	 * epoll_ctl(... ADD ...)
	 */
	for (i=1;i < argc; i++){
		argvFd = open(argv[i],O_RDWR);
		add_to_epoll(mEpollFd,argvFd);
	}

	/*
	 * epoll wait
	 73 typedef union epoll_data
	 74 {
	 75   void *ptr;
	 76   int fd;
	 77   uint32_t u32;
	 78   uint64_t u64;
	 79 } epoll_data_t;
	 80 
	 81 struct epoll_event
	 82 {
	 83   uint32_t events;	// Epoll events 
	 84   epoll_data_t data;	// User data variable 
     85 } EPOLL_PACKED;
	 */
	while(1){
		epoll_result = epoll_wait(mEpollFd,mPendingEventItems,10,-1);
		for (i=0;i < epoll_result; i++){
			printf("events = 0x%X\n",mPendingEventItems[i].events);
			int len = read(mPendingEventItems[i].data.fd, buf,256);
			buf[len] = '\0';
			printf("new data : %s",buf);
			usleep(40000);
		}
	}
}
