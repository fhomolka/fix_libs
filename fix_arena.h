/* fix_arena.h
* License: Public Domain or zlib
*
* To use this library, just include it in one C or C++ file:
* #include "fix_arena.h"
*
* Arena Allocator
*
* NOTES:
* All functions are declared `static inline` since they don't do anything complex
* The arena currently calls `malloc`, but only during `init`. This will likely be changed to take in a pre-existing buffer
* `free` exists to provide parity for anything that requires malloc and free to be redefined together, but does nothing
*/

#ifndef FIX_ARENA_H
#define FIX_ARENA_H 

#ifdef __cplusplus
extern "C"{
#endif //_cplusplus

//TODO(Fix): Maybe take in a pre-prepared buffer?
#include <stdlib.h> 
#include <string.h>

struct fix_arena_s
{
	unsigned char *memory;
	unsigned int size;
	unsigned int offset;
};
typedef struct fix_arena_s fix_arena;

//Mallocs a chunk of memory and sets it as the arena's memory
static inline void fix_arena_init(fix_arena *arena, unsigned int size)
{
	arena->memory = (unsigned char *)malloc(size);
	arena->size = size;
	arena->offset = 0;
}

//Frees the malloc'ed memory, and sets all arena info to zero
static inline void fix_arena_destroy(fix_arena *arena)
{
	free(arena->memory);
	arena->size = 0;
	arena->offset = 0;
}

//Zeroes-out the memory, returns the offset to the start of the memory
static inline void fix_arena_zero(fix_arena *arena)
{
	memset(arena->memory, 0, arena->size);
	arena->offset = 0;
}

//allocates memory within the arena. If there's not enough memory within the arena, NULL is returned.
static inline void *fix_arena_malloc(fix_arena *arena, unsigned int size)
{
	if(size + arena->offset > arena->size) return NULL; //We can't allocate that much

	void *mem = &arena->memory[arena->offset];
	arena->offset += size;
	return mem;
}

//Does nothing
static inline void fix_arena_free(fix_arena *arena, void *mem)
{
	(void)arena;
	(void)mem;
}

//Returns the memory offset to zero, treating the memory as empty. Use `zero` if you need to destroy the data.
static inline void fix_arena_free_all(fix_arena *arena)
{
	arena->offset = 0;
}

#ifdef __cplusplus
}
#endif //_cplusplus

#endif