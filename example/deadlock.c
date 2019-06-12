#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t m;

void * runthread1(void * pData){
	int i = 0;
	for (i=1;;i+=2){
		pthread_mutex_lock(&m);
		printf("odd,  %d\n",i);
		usleep(10);
		pthread_mutex_unlock(&m);
	}
}

void * runthread2(void * pData){
	int i = 0;
	for (i=0;;i+=2){
		pthread_mutex_lock(&m);
		printf("even, ---------,%d\n",i);
		usleep(10);
		pthread_mutex_unlock(&m);
	}
}

int main(int argc, char** argv){
	pthread_t  thread1, thread2;
	pthread_mutex_init(&m,0);
	pthread_create(&thread1,0, runthread1,0);
	pthread_create(&thread2,0, runthread2,0);
	sleep(3);

	printf("force to stop the thread1\n");
	pthread_cancel(thread1);
	pthread_join(thread1,(void**)0);
	pthread_join(thread2,(void**)0);
	pthread_mutex_destroy(&m);
	return 0;
}
