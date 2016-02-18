#include "uthread.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

void *foo(void *arg)
{
	int n = *(int *)arg;
	int i;
	for (i = 0; i < n; i++) {
		printf("%s i = %d\n", __func__, i);
		uthread_yield();
	}
	return NULL;
}

int main(int argc, char **argv)
{
	int n = 23;
	struct uthread_t *thread = uthread_new(foo, &n);
	int i;
	printf("%s begin\n", __func__);
	for (i = 0; i < n; i++) {
		printf("%s i = %d\n", __func__, i);
		uthread_resume(thread);
	}
	printf("%s end\n", __func__);
	return 0;
}
