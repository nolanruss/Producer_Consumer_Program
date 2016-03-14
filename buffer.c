/**
 * Producer Consumer (page 253)
 * using counting semaphores as well as a mutex lock.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#define TRUE 1
#define BUFFER_SIZE 5
#define MAX_SLEEP 5

#define MIN 0  //minimum value of an element in buffer
#define MAX 1000 //maximum value of an element in buffer

int buffer[BUFFER_SIZE];

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

int insertPointer = 0, removePointer = 0, buffer_time = 0;

void *producer(void *param);
void *consumer(void *param);

int myRand() {
   return rand() % (MAX - MIN + 1) + MIN;
}

int print_buffer(){
	int i;
	for (i=0; i< BUFFER_SIZE; i++)
		printf("slot %d: %d\n", i, buffer[i]);
	printf("\n");	
	return 0;
}

int init_buffer(){
	int i;
	for (i=0; i< BUFFER_SIZE; i++)
		buffer[i] = -1;
	return 0;
}

int insert_item(int item)
{
	/* Wait until items may be added to the buffer. */
	sem_wait(&empty);

	/* Acquire the mutex lock. */
	pthread_mutex_lock(&mutex);

	/* Insert item into the buffer */
	buffer[insertPointer] = item;
	insertPointer = (insertPointer + 1)%BUFFER_SIZE;
	buffer_time += 1;
	
	/* Print results. */
	printf("Producer %lu produced %d\n", pthread_self(), item);
	if(print_buffer()){
		printf("Buffer print fail.");
	}
	
	/* Release the mutex lock. */
	pthread_mutex_unlock(&mutex);

	/* Signal that items may be removed from the buffer. */
	sem_post(&full);

	return 0;
}

int remove_item()
{
	/* Wait until items may be removed from the buffer. */
	sem_wait(&full);

	/* Acquire the mutex lock. */
	pthread_mutex_lock(&mutex);

	/* Remove an item from the buffer */
	int temp;
	temp = buffer[removePointer];
	buffer[removePointer] = -1;
	removePointer = (removePointer + 1)%BUFFER_SIZE;
	buffer_time += 1;
	
	/* Print results. */
	printf("Consumer %lu consumed %d\n", pthread_self(), temp);
	if(print_buffer()){
		printf("Buffer print fail.");
	}
	
	/* Release the mutex lock. */
	pthread_mutex_unlock(&mutex);

	/* Signal that items may be added to the buffer. */
	sem_post(&empty);

	return 0;
}

int main(int argc, char *argv[])
{
	int sleepTime, producerThreads, consumerThreads;
	int i, j;

	if(argc != 4)
	{
		fprintf(stderr, "Useage: <sleep time> <producer threads> <consumer threads>\n");
		return -1;
	}

	sleepTime = atoi(argv[1]);
	producerThreads = atoi(argv[2]);
	consumerThreads = atoi(argv[3]);

	/* Initialize buffer to be -1 */
	init_buffer();

	/* Initialize the synchronization tools */
	printf("%d\n",pthread_mutex_init(&mutex, NULL));
	printf("%d\n",sem_init(&empty, 0, BUFFER_SIZE));
	printf("%d\n",sem_init(&full, 0, 0));
	srand(time(0));

	/* Create the producer and consumer threads */
	for(i = 0; i < producerThreads; i++)
	{
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, producer, NULL);
	}

	for(j = 0; j < consumerThreads; j++)
	{
		pthread_t tid;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr, consumer, NULL);
	}

	/* Sleep for user specified time */
	sleep(sleepTime);
	return 0;
}


void *producer(void *param)
{
	int random;
	int r;

	int current_time=0;
	int tBuffer_time=buffer_time;
	int buffer_time_diff;
	while(TRUE)
	{
		r = rand() % MAX_SLEEP;
		sleep(r);
		random = myRand();
		current_time+=r;
		tBuffer_time = buffer_time;

		printf("Producer %lu tries to insert %d at time %d\n", pthread_self(), random, current_time); 
		if(insert_item(random))
			fprintf(stderr, "Error");
		else{
			buffer_time_diff = (buffer_time - 1) - tBuffer_time;
			printf("Producer %lu successful at time %d, wait time %d\n\n", pthread_self(), (current_time + buffer_time_diff), buffer_time_diff); 
		}
	}
}

void *consumer(void *param)
{
	int r;
	int current_time=0;
	int tBuffer_time=buffer_time;
	int buffer_time_diff;
	while(TRUE)
	{
		r = rand() % MAX_SLEEP;
		sleep(r);
		current_time+=r;
		tBuffer_time = buffer_time;
		
		printf("Consumer %lu tries to consume at time %d\n", pthread_self(), current_time); 

		if(remove_item())
			fprintf(stderr, "Error Consuming");
		else{
			buffer_time_diff = (buffer_time - 1) - tBuffer_time;
			printf("Consumer %lu successful at time %d, wait time %d\n\n", pthread_self(), (current_time + buffer_time_diff), buffer_time_diff); 
		}
	}
}
