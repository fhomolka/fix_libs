/* fix_stack.h
* License: Public Domain or zlib
* 
* To use this library, remember to define FIX_STACK_IMPL in ONE C or C++ file:
* #define FIX_STACK_IMPL
* #include "fix_stack.h"
*
* Stack (LIFO)
*
* The stack works with pointers, they are all stored as void-pointers.
* The stack WILL NOT malloc and free for you, YOU are responsible for managing memory
*
* Define a custom FIX_STACK_SIZE_MAX during compile-time if you want a bigger or smaller stack size.
* Define FIX_STACK_ZERO_INIT if you really want to set all pointers to 0x0.
*/

#ifndef FIX_STACK_H 
#define FIX_STACK_H

#ifndef FIX_STACK_SIZE_MAX
#define FIX_STACK_SIZE_MAX 100
#endif //FIX_STACK_SIZE_MAX

struct fix_stack_s
{
	void *stack[FIX_STACK_SIZE_MAX];
	int itop;
};

typedef struct fix_stack_s fix_stack;

void fix_stack_init(fix_stack *s);

#endif //FIX_STACK_H


#ifdef FIX_STACK_IMPL

void fix_stack_init(fix_stack *s)
{
	s->itop = 0;
#ifdef FIX_STACK_ZERO_INIT
	for (int i = 0;i < FIX_STACK_SIZE_MAX; ++i) 
	{
		s->stack[i] = 0;
	}
#endif //FIX_STACK_ZERO_INIT
}

void fix_stack_push(fix_stack *s, void *newi)
{
	if(s->itop == FIX_STACK_SIZE_MAX - 1) return;

	s->itop += 1;
	s->stack[s->itop] = newi;
	
}

void *fix_stack_pop(fix_stack *s)
{
	if(s->itop < 0) return 0;
	void *element = s->stack[s->itop];
	s->itop -= 1;
	return element;
}

#endif //FIX_STACK_IMPL
