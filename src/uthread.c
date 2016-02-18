#include "uthread.h"
#include <ucontext.h>
#include <assert.h>
#include <stdlib.h>

static struct scheduler_t *uthread_scheduler(void);
static void uthread_main(struct uthread_t *thread);

struct uthread_t {
	struct uthread_t *caller;
	uthread_func func;
	void *arg;
	ucontext_t context;
	void *stack;
	int status;
};

struct scheduler_t {
	struct uthread_t main;
	struct uthread_t *running;
};

static struct scheduler_t *uthread_scheduler(void)
{
	static __thread struct scheduler_t *scheduler = NULL;
	if (!scheduler) {
		scheduler = (struct scheduler_t *)calloc(1, sizeof(struct scheduler_t));
	}
	return scheduler;
}

struct uthread_t *uthread_new(uthread_func func, void *arg)
{
	struct scheduler_t *scheduler = uthread_scheduler();
	struct uthread_t *thread = (struct uthread_t *)calloc(1, sizeof(struct uthread_t));
	if (!thread) {
		return NULL;
	}
	thread->func = func;
	thread->arg = arg;
	void *stack = calloc(1, UTHREAD_STACK_SIZE);
	if (!stack) {
		free(thread);
		return NULL;
	}
	thread->stack = stack;
	thread->status = UTHREAD_NEW;
	return thread;
}

void uthread_resume(struct uthread_t *thread)
{
	assert(thread);

	struct scheduler_t *scheduler = uthread_scheduler();
	struct uthread_t *running = uthread_running();
	int status = thread->status;
	if (running == thread) return;
	running->status = UTHREAD_NORMAL;
	thread->caller = running;
	thread->status = UTHREAD_RUNNING;
	scheduler->running = thread;
	switch (status) {
	case UTHREAD_NEW:
		getcontext(&thread->context);
		thread->context.uc_stack.ss_sp = thread->stack;
		thread->context.uc_stack.ss_size = UTHREAD_STACK_SIZE;
		thread->context.uc_link = &running->context;
		makecontext(&thread->context, (void (*)(void))uthread_main,
			    1, thread);
		swapcontext(&running->context, &thread->context);
		break;
	case UTHREAD_SUSPENDED:
		swapcontext(&running->context, &thread->context);
		break;
	default:
		assert(0);
	}
}

void uthread_yield(struct uthread_t *thread)
{
	assert(thread);
	assert(thread->func);
	assert(thread->status == UTHREAD_RUNNING);

	struct scheduler_t *scheduler = uthread_scheduler();
	struct uthread_t *caller = thread->caller;
	assert(scheduler->running == thread);
	caller->status = UTHREAD_RUNNING;
	thread->status = UTHREAD_SUSPENDED;
	scheduler->running = caller;
	swapcontext(&thread->context, &caller->context);
}

int uthread_status(struct uthread_t *thread)
{
	assert(thread);

	return thread->status;
}

struct uthread_t *uthread_running()
{
	struct scheduler_t *scheduler = uthread_scheduler();

	if (!scheduler->running) {
		scheduler->main.status = UTHREAD_RUNNING;
		scheduler->running = &scheduler->main;
	}
	return scheduler->running;
}

void uthread_free(struct uthread_t *thread)
{
	if (thread) {
		free(thread->stack);
		free(thread);
	}
}

static void uthread_main(struct uthread_t *thread)
{
	assert(thread);

	struct scheduler_t *scheduler = uthread_scheduler();

	thread->func(thread->arg);
	thread->status = UTHREAD_DEAD;

	struct uthread_t *running = thread->caller;
	running->status = UTHREAD_RUNNING;
	scheduler->running = running;
}
