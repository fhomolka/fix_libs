/* fix_swapback.h
* License: BSD0
* 
* To use this library, just include the header
* #include "fix_swapback.h"
*
* Fixed-Size Swapback Array
*/ 

#ifndef FIX_SWAPBACK_H
#define FIX_SWAPBACK_H

#define fix_swapback(T, size) struct { unsigned int len; T data[size]; unsigned int maxlen; }
#define fix_swapback_init(arr, size) {arr.len = 0; arr.maxlen = size;}
#define fix_swapback_append(arr, new) do{if(arr.len == arr.maxlen) {break;} arr.data[arr.len] = new; arr.len += 1;}while(0)
#define fix_swapback_remove(arr, idx) do{if(arr.len < idx){break;} arr.data[idx] = arr.data[arr.len - 1]; arr.len -= 1; }while(0)

#endif //FIX_SWAPBACK