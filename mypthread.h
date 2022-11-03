// File:	mypthread_t.h

// List all group members' names:
// iLab machine tested on:

#ifndef MYTHREAD_T_H
#define MYTHREAD_T_H

#define _GNU_SOURCE

/* in order to use the built-in Linux pthread library as a control for benchmarking, you have to comment the USE_MYTHREAD macro */
#define USE_MYTHREAD 1

/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/time.h>
#include <stdatomic.h>

typedef uint mypthread_t;

enum status
{
	READY,
	RUNNING,
	BLOCKED,
	EXIT
};

enum lock_status
{
	UNLOCKED,
	LOCKED
};

typedef struct threadControlBlock
{
	// YOUR CODE HERE	
	
	// thread Id
	// thread status
	// thread context
	// thread stack
	// thread priority
	// And more ...

	mypthread_t id;
	enum status status;
	ucontext_t *mpt_cntxt;
	int priority;	
	struct timeval time;
	int quantum;
	void *value_ptr;
	int yield;

} tcb;

/* mutex struct definition */
typedef struct mypthread_mutex_t
{
	atomic_flag lock;
	struct Queue *blocked_queue;
	
} mypthread_mutex_t;


// Feel free to add your own auxiliary data structures (linked list or queue etc...)
typedef struct QNode
{
	tcb *tcb;
	struct QNode *next;
} qnode;

typedef struct Queue
{
	struct QNode *front, *rear;
} queue;

struct QNode *newNode(tcb *item);

struct Queue *createQueue();

void enqueue(struct Queue *q, tcb *item);

tcb *dequeue(struct Queue *q);

static void schedule();
static void sched_rr(int interval_ms);
static void sched_PSJF();
static void handler();
static void blockSignalProf(sigset_t *set);
static void unblockSignalProf(sigset_t *set);
static void stoptimer();
static void runtimer(int interval_ms);
queue PriorityArray[4];


/* Function Declarations: */

/* create a new thread */
int mypthread_create(mypthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg);

/* current thread voluntarily surrenders its remaining runtime for other threads to use */
int mypthread_yield();

/* terminate a thread */
void mypthread_exit(void *value_ptr);

/* wait for thread termination */
int mypthread_join(mypthread_t thread, void **value_ptr);

/* initialize a mutex */
int mypthread_mutex_init(mypthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);

/* aquire a mutex (lock) */
int mypthread_mutex_lock(mypthread_mutex_t *mutex);

/* release a mutex (unlock) */
int mypthread_mutex_unlock(mypthread_mutex_t *mutex);

/* destroy a mutex */
int mypthread_mutex_destroy(mypthread_mutex_t *mutex);

#ifdef USE_MYTHREAD
#define pthread_t mypthread_t
#define pthread_mutex_t mypthread_mutex_t
#define pthread_create mypthread_create
#define pthread_exit mypthread_exit
#define pthread_join mypthread_join
#define pthread_mutex_init mypthread_mutex_init
#define pthread_mutex_lock mypthread_mutex_lock
#define pthread_mutex_unlock mypthread_mutex_unlock
#define pthread_mutex_destroy mypthread_mutex_destroy
#endif

#endif
