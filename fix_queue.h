/* fix_queue.h
* License: Public Domain or zlib
* 
* To use this library, remember to define FIX_QUEUE_IMPL in ONE C or C++ file:
* #define FIX_QUEUE_IMPL
* #include "fix_queue.h"
*
* Circular queue (ring buffer) style
*
* The queue works with pointers, they are all stored as void-pointers.
* The queue WILL NOT malloc and free for you, YOU are responsible for managing memory
*
* Define a custom FIX_QUEUE_SIZE_MAX during compile-time if you want a bigger or smaller queue size.
* Define FIX_QUEUE_ZERO_INIT if you really want to set all pointers to 0x0.
*/

#ifndef FIX_QUEUE_H
#define FIX_QUEUE_H

#ifdef __cplusplus
extern "C"{
#endif //_cplusplus

#ifndef FIX_QUEUE_SIZE_MAX
#define FIX_QUEUE_SIZE_MAX 100
#endif //FIX_QUEUE_SIZE_MAX

struct at_queue_s
{
	void *queue[FIX_QUEUE_SIZE_MAX];
	unsigned int ifront;
	unsigned int iback;
};

typedef struct at_queue_s fix_queue;

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //FIX_QUEUE_H


// Implementation Start
#ifdef FIX_QUEUE_IMPL

#ifdef __cplusplus
extern "C"{
#endif //_cplusplus

// Resets the queue to an initial state. You could use this to 'empty-out' the queue
void fix_queue_init(fix_queue *q)
{
	q->ifront = -1;
	q->iback = -1;

#ifdef FIX_QUEUE_ZERO_INIT
	for (int i = 0; i < FIX_QUEUE_SIZE_MAX; ++i)
	{
		q->queue[i] = 0;
	}
#endif //FIX_QUEUE_ZERO_INIT
}

void fix_queue_enqueue(fix_queue *q, void *newi)
{
	if((q->ifront == q->iback + 1) || (q->ifront == 0 && q->iback == FIX_QUEUE_SIZE_MAX - 1)) return;

	if (q->ifront == -1) q->ifront = 0;

	//TODO(Fix): Replace with ++q->iback % FIX_QUEUE_SIZE_MAX?
	q->iback = (q->iback + 1) % FIX_QUEUE_SIZE_MAX;
	q->queue[q->iback] = newi;
}

void *fix_queue_dequeue(fix_queue *q)
{
	if(q->ifront == -1) return 0;

	void *item = q->queue[q->ifront];

	if(q->ifront == q->iback) q->ifront = q->iback = -1;
	else q->ifront = (q->ifront + 1) % FIX_QUEUE_SIZE_MAX;
	return item;
}

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //FIX_QUEUE_IMPL