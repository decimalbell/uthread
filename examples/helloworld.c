#include "uthread.h"
#include <stdio.h>

void *hello_world(void *arg)
{
	struct uthread_t *running = uthread_running();
	printf("uthread %p status %d\n", running, uthread_status(running));
	printf("hello world\n");
	return NULL;
}

int main(int argc, char **argv)
{
	struct uthread_t *running = uthread_running();
	printf("uthread %p status %d\n", running, uthread_status(running));

	struct uthread_t *thread = uthread_new(hello_world, NULL);
	uthread_resume(thread);

	running = uthread_running();
	printf("uthread %p status %d\n", running, uthread_status(running));
	return 0;
}
