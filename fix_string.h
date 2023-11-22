/* fix_string.h
* License: Public Domain or zlib
* 
* To use this library, remember to define FIX_STRING_IMPL in ONE C or C++ file:
* #define FIX_STRING_IMPL
* #include "fix_string.h"
*
* Nice strings
*/ 

#ifndef FIX_STRING_H
#define FIX_STRING_H

struct fix_string_s
{
	char *data;
	unsigned int len;
};

typedef struct fix_string_s fix_string;

#endif //FIX_STRING_H

#ifdef FIX_STRING_IMPL
#include <string.h>
#include <stdio.h>

fix_string cstring_to_fix_string(const char* text)
{
	fix_string new_string;
	new_string.data = (char*)text;
	new_string.len = strlen(text);

	return new_string;
}

int fix_string_compare(fix_string *first, fix_string *second)
{
	if (first->len != second->len) return 0;
	return memcmp(first->data, second->data, first->len) == 0;
}

int fix_string_obj_compare(fix_string *first, fix_string *second)
{
	return first->len == second->len && first->data == second->data;
}

fix_string fix_string_substring(fix_string *str, unsigned int start, unsigned int len)
{
	fix_string substr;

	substr.data = str->data + start;
	substr.len = len;

	return substr;
}

void fix_string_print(fix_string *str)
{
	printf("%s\n", str->data);
}

void fix_string_stupid_print(fix_string *str)
{
	for (unsigned int i = 0; i < str->len; ++i)
	{
		putc(str->data[i], stdout);
	}
}

void fix_string_stupid_println(fix_string *str)
{
	fix_string_stupid_print(str);
	putc('\n', stdout);
}

#endif //FIX_STRING_IMPL