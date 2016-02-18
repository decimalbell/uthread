#pragma once

#include <sys/types.h>
#include <sys/socket.h>

#define UTHREAD_NEW       0
#define UTHREAD_NORMAL    1
#define UTHREAD_RUNNING   2
#define UTHREAD_SUSPENDED 3
#define UTHREAD_DEAD      4

#define UTHREAD_STACK_SIZE 128 * 1024

typedef void *(*uthread_func)(void *);

struct uthread_t;

struct uthread_t *uthread_new(uthread_func func, void *arg);
void uthread_resume(struct uthread_t *thread);
void uthread_yield(void);
int uthread_status(struct uthread_t *thread);
struct uthread_t *uthread_running(void);
void uthread_free(struct uthread_t *thread);
