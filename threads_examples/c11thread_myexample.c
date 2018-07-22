#include <stdio.h>
#include <stdlib.h>
#include <threads.h> 
#include <unistd.h>
#include <string.h>

int print_message_function( void *ptr );

struct msg
{
	const char *text;
	mtx_t *mutex;
};

int 	main()
{
    thrd_t thread1, thread2;
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    int  iret1, iret2;
    mtx_t sharedlock;
	mtx_init(&sharedlock, NULL);
	struct msg structs[2];

	structs[0].text = message1;
	structs[0].mutex = &sharedlock;
	structs[1].text = message2;
	structs[1].mutex = &sharedlock;

    iret1 = thrd_create( &thread1, print_message_function, (void*) &structs[0]);
    if(iret1 != thrd_success)
    {
        fprintf(stderr,"Error - thrd_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }

    iret2 = thrd_create( &thread2, print_message_function, (void*) &structs[1]);
    if(iret2 != thrd_success)
    {
        fprintf(stderr,"Error - thrd_create() return code: %d\n",iret2);
        exit(EXIT_FAILURE);
    }
    mtx_lock(&sharedlock);
    printf("thrd_create() for thread 1 returns: %d\n",iret1);
    printf("thrd_create() for thread 2 returns: %d\n",iret2);
    mtx_unlock(&sharedlock);

    thrd_join( thread1, NULL);
    thrd_join( thread2, NULL); 
    exit(EXIT_SUCCESS);
}

int print_message_function( void *ptr )
{
    struct msg *message;
    message = (struct msg *) ptr;
    mtx_lock(message->mutex);
    write(1, message->text, strlen(message->text));
    write(1, "\n", 1);
    mtx_unlock(message->mutex);
    return (0);
}