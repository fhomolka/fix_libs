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

unsigned int fix_string_count_char(fix_string *str, char c);

fix_string *fix_string_split_by_char(fix_string *str, char delimiter, /* out */ unsigned int *count);

int fix_string_find_char_from_left(fix_string *str, char c);
int fix_string_find_char_from_right(fix_string *str, char c);
int fix_string_find_any_char_from_left(fix_string *str, char *chars);
int fix_string_find_any_char_from_right(fix_string *str, char *chars);
int fix_string_find_cstring_from_left(fix_string *str, const char *cstr);
int fix_string_find_cstring_from_right(fix_string *str, const char *cstr);
int fix_string_find_any_cstring_from_left(fix_string *str, char **cstrings, unsigned int count);
int fix_string_find_any_cstring_from_right(fix_string *str, char **cstrings, unsigned int count);
int fix_string_find_string_from_left(fix_string *str, fix_string *substr);
int fix_string_find_string_from_right(fix_string *str, fix_string *substr);

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

unsigned int fix_string_count_char(fix_string *str, char c)
{
	unsigned int count = 0;

	for (unsigned int i = 0; i < str->len; ++i)
	{
		if (str->data[i] == c) count += 1;
	}

	return count;
}

fix_string *fix_string_split_by_char(fix_string *str, char delimiter, /* out */ unsigned int *count)
{
	*count = fix_string_count_char(str, delimiter);
	if (str->data[str->len - 1] != delimiter)
	{
		*count += 1;
	}

	fix_string *substrings = malloc(*count * sizeof(fix_string));

	int filled_count = 0;
	int last_delim = -1;

	for (unsigned int i = 0; i < str->len; ++i)
	{
		if (str->data[i] != delimiter) continue;

		substrings[filled_count] = fix_string_substring(str, last_delim + 1, i - last_delim - 1);
		filled_count += 1;

		last_delim = i;
	}

	if (str->data[str->len - 1] != delimiter)
	{
		substrings[filled_count] = fix_string_substring(str, last_delim + 1, str->len - 1 - last_delim);
	}
	
	return substrings;
}

int fix_string_find_char_from_left(fix_string *str, char c)
{
	for (unsigned int i = 0; i < str->len; ++i)
	{
		if (str->data[i] == c) return i;
	}

	return -1;
}

int fix_string_find_char_from_right(fix_string *str, char c)
{
	for (int i = str->len - 1; i >= 0; --i)
	{
		if (str->data[i] == c) return i;
	}

	return -1;
}

int fix_string_find_any_char_from_left(fix_string *str, char *chars)
{
	int leftmost_idx = -1;
	unsigned int len = strlen(chars);
	for (unsigned int i = 0; i < len; ++i)
	{
		int ret = fix_string_find_char_from_left(str, chars[i]);
		if (ret != -1 && (leftmost_idx == -1 || leftmost_idx > ret)) leftmost_idx = ret;
	}
	return leftmost_idx;
}

int fix_string_find_any_char_from_right(fix_string *str, char *chars)
{
	int rightmost_idx = -1;
	unsigned int len = strlen(chars);
	for (unsigned int i = 0; i < len; ++i)
	{
		int ret = fix_string_find_char_from_right(str, chars[i]);
		if (rightmost_idx < ret) rightmost_idx = ret;
	}
	return rightmost_idx;
}

int fix_string_find_cstring_from_left(fix_string *str, const char *cstr)
{
	unsigned int len = strlen(cstr);
	if (str->len < len) return -1;
	for (unsigned int i = 0; i + len <= str->len; ++i)
	{
		if (memcmp(str->data + i, cstr, len) == 0) return i;
	}

	return -1;
}

int fix_string_find_cstring_from_right(fix_string *str, const char *cstr)
{
	unsigned int len = strlen(cstr);
	if (str->len < len) return -1;
	for (int i = str->len - 1; i - len >= 0; --i) //Throws a warning, but works incorrectly when len is casted to int
	{
		if (memcmp(str->data + i, cstr, len) == 0) return i;
	}

	return -1;
}

int fix_string_find_any_cstring_from_left(fix_string *str, char **cstrings, unsigned int count)
{
	int leftmost_idx = -1;
	for (unsigned int i = 0; i < count; ++i)
	{
		int ret = fix_string_find_cstring_from_left(str, cstrings[i]);
		if (ret != -1 && (leftmost_idx == -1 || leftmost_idx > ret)) leftmost_idx = ret;
	}
	return leftmost_idx;
}

int fix_string_find_any_cstring_from_right(fix_string *str, char **cstrings, unsigned int count)
{
	int rightmost_idx = -1;
	for (unsigned int i = 0; i < count; ++i)
	{
		int ret = fix_string_find_cstring_from_right(str, cstrings[i]);
		if (rightmost_idx < ret) rightmost_idx = ret;
	}
	return rightmost_idx;
}

int fix_string_find_string_from_left(fix_string *str, fix_string *substr)
{
	if (str->len < substr->len) return -1;
	for (unsigned int i = 0; i + substr->len <= str->len; ++i)
	{
		if (memcmp(str->data + i, substr->data, substr->len) == 0) return i;
	}

	return -1;
}

int fix_string_find_string_from_right(fix_string *str, fix_string *substr)
{
	if (str->len < substr->len) return -1;
	for (int i = str->len - 1; substr->len >= 0; --i) //Throws a warning, but works incorrectly when len is casted to int
	{
		if (memcmp(str->data + i, substr->data, substr->len) == 0) return i;
	}

	return -1;
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