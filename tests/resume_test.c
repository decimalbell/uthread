#include "uthread.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

struct uthread_t *uth1;
struct uthread_t *uth2;
struct uthread_t *uth3;

void *uthread1(void *arg);
void *uthread2(void *arg);
void *uthread3(void *arg);

void *uthread1(void *arg)
{
	printf("%s begin\n", __func__);
	uthread_resume(uth2);
	printf("%s end\n", __func__);

	return NULL;
}

void *uthread2(void *arg)
{
	printf("%s begin\n", __func__);
	uthread_resume(uth3);
	printf("%s end\n", __func__);

	return NULL;
}

void *uthread3(void *arg)
{
	printf("%s begin\n", __func__);
	printf("%s end\n", __func__);

	return NULL;
}

int main(int argc, char **argv)
{
	uth1 = uthread_new(uthread1, NULL);
	uth2 = uthread_new(uthread2, NULL);
	uth3 = uthread_new(uthread3, NULL);

	printf("%s begin\n", __func__);
	uthread_resume(uth1);
	printf("%s end\n", __func__);

	return 0;
}
