#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void *print_message_function( void *ptr );

struct msg
{
	const char *text;
	pthread_mutex_t *mutex;
};

int 	main()
{
    pthread_t thread1, thread2;
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    int  iret1, iret2;
    pthread_mutex_t sharedlock;
	pthread_mutex_init(&sharedlock, NULL);
	struct msg structs[2];

	structs[0].text = message1;
	structs[0].mutex = &sharedlock;
	structs[1].text = message2;
	structs[1].mutex = &sharedlock;

    iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) &structs[0]);
    if(iret1)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }


    iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) &structs[1]);
    if(iret2)
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
        exit(EXIT_FAILURE);
    }
    pthread_mutex_lock(&sharedlock);
    printf("pthread_create() for thread 1 returns: %d\n",iret1);
    printf("pthread_create() for thread 2 returns: %d\n",iret2);
    pthread_mutex_unlock(&sharedlock);

    pthread_join( thread1, NULL);
    pthread_join( thread2, NULL); 

    exit(EXIT_SUCCESS);
}

void *print_message_function( void *ptr )
{
    struct msg *message;
    message = (struct msg *) ptr;
    pthread_mutex_lock(message->mutex);
    write(1, message->text, strlen(message->text));
    write(1, "\n", 1);
    pthread_mutex_unlock(message->mutex);
}