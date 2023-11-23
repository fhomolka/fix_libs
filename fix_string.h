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

fix_string cstring_to_fix_string(const char* text);

fix_string fix_string_clone(fix_string *first);
void fix_string_clone_free(fix_string *str);

int fix_string_compare(fix_string *first, fix_string *second);
int fix_string_obj_compare(fix_string *first, fix_string *second);
fix_string fix_string_substring(fix_string *str, unsigned int start, unsigned int len);
void fix_string_print(char *fmt, ...);

#endif //FIX_STRING_H

#ifdef FIX_STRING_IMPL
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

fix_string cstring_to_fix_string(const char* text)
{
	fix_string new_string;
	new_string.data = (char*)text;
	new_string.len = strlen(text);

	return new_string;
}

fix_string fix_string_clone(fix_string *first)
{
	fix_string clone;
	clone.len = first->len;

	clone.data = malloc(clone.len * sizeof(char));

	strcpy(clone.data, first->data);

	return clone;
}

void fix_string_clone_free(fix_string *str)
{
	free(str->data);
	str->len = 0;
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

#include <stdarg.h>

void fix_string_print(char *fmt, ...)
{
	//TODO(Fix): Buffer and puts buffer
	va_list args;
	va_start(args, fmt);

	while(*fmt)
	{
		if (*fmt != '%')
		{
			putc(*fmt, stdout);
			fmt += 1;
			continue;
		}

		fmt += 1;
		switch(*fmt)
		{
			case '%':
			{
				putc('%', stdout);
				break;
			}
			case 'c':
			{
				int c = va_arg(args, int);
				putc(c, stdout);
				break;
			}
			case 'd':
			case 'i':
			{
				int n = va_arg(args, int);
				char buf[64];
				int len = snprintf(buf, sizeof(buf), "%i", n);

				for (int i = 0; i < len; ++i)
				{
					putc(buf[i], stdout);
				}
				break;
			}
			case 'f':
			{
				double n = va_arg(args, double);
				char buf[64];
				int len = snprintf(buf, sizeof(buf), "%f", n);
				for (int i = 0; i < len; ++i)
				{
					putc(buf[i], stdout);
				}
				break;
			}
			case 's': //cstring
			{
				char *str = va_arg(args, char*);
				
				while(*str)
				{
					putc(*str, stdout);
					str += 1;
				}
				break;
			}
			case 'z': //fix_string
			{
				fix_string str = va_arg(args, fix_string);
				for (unsigned int i = 0; i < str.len; ++i)
				{
					putc(str.data[i], stdout);
				}
			}
		}
		fmt += 1;
	}

	va_end(args);
}

#endif //FIX_STRING_IMPL