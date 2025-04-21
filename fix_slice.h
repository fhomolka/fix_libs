/* fix_slice.h
* License: Public Domain or BSD0
* 
* To use this library, just include the header
* All functions are just macros
* #include "fix_slice.h"
*
* Slices for C-Arrays
*
* Example Usage:
* int arr[3] = {1, 2 ,3};
* fix_slice(int) my_slice;
* fix_slice_init(my_slice, arr, 3);
*/ 

#ifndef FIX_SLICE_H
#define FIX_SLICE_H

#define fix_slice(T) struct { unsigned int len; T *data; }
#define fix_slice_init(_slice, _start, _l) { _slice.data = _start; _slice.len = _l; }
#define fix_slice_init_start_end(_slice, arr, _start, _end) { _slice.data = arr[_start]; _slice.len = _end - _start; }
#define fix_slice_subslice_len(_subslice, _slice, _start, _l) { _subslice.data = _slice.data[_start]; _subslice.len = _l; }
#define fix_slice_subslice_start_end(_subslice, _slice, _start, _end) { _subslice.data = _slice.data[_start]; _subslice.len = _end - _start;}

#endif

