#include <sys/inotify.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc,char** argv){
	int mInotifyFd ;
	int res;
	char event_buf[512];
	struct inotify_event *event;
	char * filename;
	int event_pos = 0;
	int event_size;
	char devname[128];
		
	if(argc < 2){
		printf("Usage: xx <dir1> [dir2] [dir3] ...\n");
		return -1;
	}
	
	strcpy(devname, argv[1]);
	filename = devname  + strlen(argv[1]);
	*filename ++= '/';
	/*
	 *inotify_init 
	 */
	mInotifyFd = inotify_init();
	/*
	 *inotify_add_watch 
	 */
	res = inotify_add_watch(mInotifyFd,argv[1],IN_DELETE | IN_CREATE);
	
	while(1){
	event_size = 0;
	event_pos = 0;
	/*
	 *inotify read 
	 */
	res = read(mInotifyFd, event_buf, sizeof(event_buf));
	if (res < (int)sizeof(*event)){
		if (errno ==EINTR)
			return 0;
		printf("Could not get event\n");
		return -1;
	}
	/* parse data which read form inotify
	 */
	while(res >= (int)sizeof(*event)){
		event = (struct inotify_event*)(event_buf + event_pos);
		if (event->len){
			strcpy(filename, event->name);
			if (event->mask & IN_CREATE){
				printf("Open dir %s\n",filename);
			}else{
				printf("Remove dir %s\n",filename);
			}
		}
		event_size = sizeof(*event) + event->len;
		res -= event_size;
		event_pos += event_size;
	}
	}
	return 0;
}
