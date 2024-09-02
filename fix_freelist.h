/* fix_freelist.h
* License: Public Domain or zlib
*
* To use this library, just include it in one C or C++ file:
* #include "fix_freelist.h"
*
* Freelist Allocator
*
* NOTES:
* All functions are declared `static inline` for ease of use. If you do notice a performance hit, please do report it.
* The freelist currently calls `malloc`, but only during `init`. This will likely be changed to take in a pre-existing buffer
*/

#ifndef FIX_LIST_ALLOC_H
#define FIX_LIST_ALLOC_H 

#ifdef __cplusplus
extern "C"{
#endif //_cplusplus


//TODO(Fix): Maybe take in a pre-prepared buffer?
#include <stdlib.h> 

struct fix_freelist_node_s
{
	unsigned int block_offset;
	unsigned int block_size;
	struct fix_freelist_node_s *prev;
	struct fix_freelist_node_s *next;
};
typedef struct fix_freelist_node_s fix_freelist_node;


struct fix_freelist_s
{
	unsigned char *memory;
	unsigned int size;
	fix_freelist_node *first_node;
	fix_freelist_node *last_node;
};
typedef struct fix_freelist_s fix_freelist;

//Mallocs a block of memory and sets it as the freelist's memory
static inline void fix_freelist_init(fix_freelist *fl, unsigned int size)
{
	fl->memory = (unsigned char *)malloc(size * sizeof(char));
	fl->size = size;

	fl->first_node = NULL;
	fl->last_node = NULL;
}

//Frees the malloc'ed memory, and sets all freelist info to zero
static inline void fix_freelist_destroy(fix_freelist *fl)
{
	free(fl->memory);
	fl->size = 0;
}

//allocates memory within the freelist. If there's not enough memory within the fl, NULL is returned.
static inline void *fix_freelist_malloc(fix_freelist *fl, unsigned int size)
{
	//Nothing allocated yet
	if(!fl->last_node)
	{
		fix_freelist_node *new_node = (fix_freelist_node*)&fl->memory[0];
		new_node->prev = NULL;
		new_node->next = NULL;
		new_node->block_size = size;
		new_node->block_offset = sizeof(*new_node);
		fl->last_node = new_node;
		fl->first_node = new_node;
		return &fl->memory[new_node->block_offset];
	}

	//We can fit it at the end
	if(fl->last_node->block_offset + fl->last_node->block_size + sizeof(fix_freelist_node) < fl->size)
	{
		unsigned int node_offset = fl->last_node->block_offset + fl->last_node->block_size;
		fix_freelist_node *new_node = (fix_freelist_node*)&fl->memory[node_offset];
		fl->last_node->next = new_node;
		new_node->prev = fl->last_node;
		new_node->next = NULL;
		new_node->block_offset = node_offset + sizeof(*new_node);
		new_node->block_size = size;

		fl->last_node = new_node;
		return &fl->memory[new_node->block_offset];
	}

	//Maybe at the start?
	if(fl->first_node != (fix_freelist_node*)&fl->memory[0])
	{
		unsigned int free_space = fl->first_node->block_offset - sizeof(fix_freelist_node);
		if(free_space >= size + sizeof(fix_freelist_node)) 
		{
			fix_freelist_node *node = (fix_freelist_node*)&fl->memory[0];
			node->prev = NULL;
			node->next = fl->first_node;
			node->next->prev = node;
			node->block_size = size;
			node->block_offset = sizeof(*node);
			fl->first_node = node;
			return &fl->memory[node->block_offset];
		}
	}

	//Can we fit it anywhere in the middle?
	fix_freelist_node *node = fl->first_node;
	while(node->next)
	{
		unsigned int free_space = (node->next->block_offset) - (node->block_offset + node->block_size);
		if(free_space < size + sizeof(fix_freelist_node)) {node = node->next; continue;}

		unsigned int node_offset = node->block_offset + node->block_size;
		fix_freelist_node *new_node = (fix_freelist_node*)&fl->memory[node_offset];

		new_node->prev = node;
		new_node->next = node->next;

		node->next = new_node;
		new_node->next->prev = new_node;

		new_node->block_offset = node_offset + sizeof(*new_node);
		new_node->block_size = size;

		return &fl->memory[new_node->block_offset];
	}

	return NULL;
}

//Frees the memory by removing it from the linked list. Doesn't zero it.
static inline void fix_freelist_free(fix_freelist *fl, void *mem)
{
	fix_freelist_node *node = (fix_freelist_node*)(((unsigned char*)mem) - sizeof(fix_freelist_node));
	if(node->prev) node->prev->next = node->next;
	if(node->next) node->next->prev = node->prev;

	if(node == fl->last_node) fl->last_node = node->prev;
	if(node == fl->first_node) fl->first_node = node->next;
}

static inline void fix_freelist_free_all(fix_freelist *fl)
{
	fl->first_node = NULL;
	fl->last_node = NULL;
}

#ifdef __cplusplus
}
#endif //_cplusplus

#endif //FIX_LIST_ALLOC_H