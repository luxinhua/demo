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
	struct epoll_event eventItem;
	char buf[256];
	int mEpollFd;
	int i;
	int epoll_result;
	int result;
	
	int event_size;
	int event_pos;
	char event_buf[512];
	int inotifyFd;
	char *filename;
	struct inotify_event *event;
		
	if (argc<2){
		printf("Usage: xxx <dir1> [dir2] [dir3] ...\n");
		return 0;
	}
	inotifyFd = inotify_init();
	result = inotify_add_watch(mInotifyFd,argv[1],IN_DELETE | IN_CREATE);

	mEpollFd = epoll_create(16);

	memset(&eventItem,0,sizeof(eventItem));
	eventItem.events = EPOLLIN;
	eventItem.data.fd = inotifyFd;
	eventItem.data.u32 = EPOLL_ID_INOTIFY;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,inotifyFd,&eventItem)

	while(1){
		epoll_result = epoll_wait(mEpollFd,mPendingEventItems,10,-1);
		if (epoll_result > 0){
			if (mPendingEventItems[i].data.u32 == EPOLL_ID_INOTIFY){
				event_size = 0;
				event_pos = 0;
				result = read(mInotifyFd, event_buf, sizeof(event_buf));
				if (result < (int)sizeof(*event))
					return -1;
				while (result >= (int)sizeof(*event)){
					event = (struct inotify_event*)(event_buf + event_pos);
					if (event->len){
						strcpy(filename, event->name);
						if(event->mask & IN_CREATE){
							printf("Open dir %s \n",filename);
						}else{
							printf("Remove dir %s \n",filename);
						}
					}
					event_size = sizeof(*event) + event->len;
					result = result - event->size;
					event_pos = event_pos + event_size;
				}
			}	
		}		
	}
}

