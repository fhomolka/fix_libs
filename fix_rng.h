/* fix_rng.h
* License: Public Domain or zlib
* 
* To use this library, remember to define FIX_RNG_IMPL in ONE C or C++ file:
* #define FIX_RNG_IMPL
* #include "fix_rng.h"
*
* Doom-style Pseudo-random number generator
*/ 

#ifndef FIX_RNG_H
#define FIX_RNG_H

//We're tracking the offset in the rng table.
typedef unsigned char fix_rng_state;

extern unsigned char fix_rng_table[256];

/* Sets the state to the random seed */
void fix_rng_set_seed(fix_rng_state *s, int seed);
/* Grabs a random unsigned byte and progresses the state */
unsigned char fix_rng_get_u8(fix_rng_state *s);
/* Grabs a random signed byte and progresses the state */
signed char fix_rng_get_i8(fix_rng_state *s);
/* Grabs a random float and progresses the state */
float fix_rng_get_f32(fix_rng_state *s);
/* Grabs a random "bool" and progresses the state */
int fix_rng_get_bool(fix_rng_state *s);
/* Grabs a random number between included min and included max */
int fix_rng_get_range(fix_rng_state *s, int min, int max);
#endif 

#ifdef FIX_RNG_IMPL

void fix_rng_set_seed(fix_rng_state *s, int seed)
{
	*s = seed % 255;
}

unsigned char fix_rng_get_u8(fix_rng_state *s)
{
	*s += 1 % 255;
	return fix_rng_table[*s];
}

signed char fix_rng_get_i8(fix_rng_state *s)
{
	return (signed char)fix_rng_get_u8(s);
}

float fix_rng_get_f32(fix_rng_state *s)
{
	unsigned char ret = fix_rng_get_u8(s);
	return (float)ret / 255;
}

int fix_rng_get_bool(fix_rng_state *s)
{
	return fix_rng_get_f32(s) >= 0.5f;
}

int fix_rng_get_range(fix_rng_state *s, int min, int max)
{
	return (fix_rng_get_u8(s) % max) + min;
}

unsigned char fix_rng_table[256] = 
{
	 33, 140,  44, 244,  13, 128,  77, 115, 
	186,  57,  80, 224, 114, 170,  12, 129, 
	192, 164,  58, 147,  16, 112,  52,  47, 
	217,  95,  87, 167,  23,  59, 113, 236, 
	210,  19, 118, 248, 252, 101, 125,  14, 
	174, 116, 117, 241,  84,  53,   8,  66, 
	151,  26, 240, 198, 200, 126, 190, 195, 
	137, 225, 150,  89,  45, 229,  37, 163, 
	179, 213, 105,  50, 123,  92, 104, 109, 
	201,  91, 207,   6, 134, 184,  17, 223, 
	145, 136, 168, 232,  75, 169, 110, 106, 
	 48, 161,  24,  56,  96, 122, 165,  81, 
	 85, 237, 162, 227, 255,  41, 157, 139, 
	 55, 160,  42, 175, 142, 127,  10, 206, 
	203, 180,  60,  15,  86, 121, 158,  39, 
	141,  88, 234,  82, 193, 166, 124, 194, 
	199, 120, 211,  98, 233,  76, 187, 103, 
	  2, 209, 131,  69, 171,   1, 221,  22, 
	 34, 251,   7, 191,  93,  61,   0,  99, 
	196,  51, 230, 102,  94, 107, 148, 204, 
	181,  65,  46,  40, 202, 189, 183,  97, 
	146, 155, 212, 156, 242,  90,  68, 226, 
	216,  70,   9,   4, 172,  29,  73,  54, 
	152, 218,  27, 215,  28, 111,  21,  18, 
	228, 185,  79,  20, 173,  35, 135, 176, 
	 30, 250, 243, 143, 197, 214, 220, 231, 
	 38, 182,  74, 247,  49,  11, 222,   5, 
	 36, 238, 249, 178, 235,  78, 138,  83, 
	108, 177, 159,  32, 149, 154, 72,  254, 
	119, 144,  31, 132,   3, 219, 239, 100, 
	130,  64,  43, 133, 245, 188, 246,  63, 
	208, 253,  25, 205,  71,  62, 153,  67
};

#endif 
