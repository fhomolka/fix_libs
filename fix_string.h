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
const char *fix_string_to_cstring(fix_string *str);

fix_string fix_string_clone(fix_string *first);
void fix_string_clone_free(fix_string *str);

/* Are strings the same */
int fix_string_compare(fix_string *first, fix_string *second);
/* Are these the same fix_string */
int fix_string_obj_compare(fix_string *first, fix_string *second);
fix_string fix_string_substring(fix_string *str, unsigned int start, unsigned int len);

/* Returns how many times a character appears */
unsigned int fix_string_count_char(fix_string *str, char c);

/* Returns an array of substrings split by delimiter */
fix_string *fix_string_split_by_char(fix_string *str, char delimiter, /* out */ unsigned int *count);

/* Finds the first instance of the character from left */
int fix_string_find_char_from_left(fix_string *str, char c);
/* Finds the first instance of the character from right */
int fix_string_find_char_from_right(fix_string *str, char c);
/* Finds all instances of the character from left */
int fix_string_find_any_char_from_left(fix_string *str, char *chars);
/* Finds all instances of the character from right */
int fix_string_find_any_char_from_right(fix_string *str, char *chars);
/* Finds the first instance of cstring substring from left */
int fix_string_find_cstring_from_left(fix_string *str, const char *cstr);
/* Finds the first instance of cstring substring from right */
int fix_string_find_cstring_from_right(fix_string *str, const char *cstr);
/* Finds all instances of cstring substring from left */
int fix_string_find_any_cstring_from_left(fix_string *str, char **cstrings, unsigned int count);
/* Finds all instances of cstring substring from right */
int fix_string_find_any_cstring_from_right(fix_string *str, char **cstrings, unsigned int count);
/* Finds all instances of fix_string substring from left */
int fix_string_find_string_from_left(fix_string *str, fix_string *substr);
/* Finds all instances of fix_string substring from right */
int fix_string_find_string_from_right(fix_string *str, fix_string *substr);

/* Returns whether string contains a character */
int fix_string_contains_char(fix_string *str, char c);
/* Returns whether string contains the cstring substring */
int fix_string_contains_cstring(fix_string *str, char *cstr);
/* Returns whether string contains fix_string substring */
int fix_string_contains_string(fix_string *str, fix_string *substr);

/* Returns a substring trimmed to before the instance of the character */
fix_string fix_string_trim_from_left_by_char(fix_string *str, char c);

/* Returns a substring trimmed to before the instance of the character */
fix_string fix_string_trim_from_right_by_char(fix_string *str, char c);

/* Attempts to parse string as an int */
int fix_string_to_int(fix_string *str);

/* Printf-like print that accepts fix_string as %z in fmt */
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

const char *fix_string_to_cstring(fix_string *str)
{
	char *cstring = malloc(str->len + 1 * sizeof(char)); // +1 for null-term
	memcpy(cstring, str->data, str->len);
	cstring[str->len] = '\0';

	return cstring;
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
	if(*count == 0) return NULL;
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

int fix_string_contains_char(fix_string *str, char c)
{
	return fix_string_find_char_from_left(str, c) != -1;
}

int fix_string_contains_cstring(fix_string *str, char *cstr)
{
	return fix_string_find_cstring_from_left(str, cstr) != -1;
}

int fix_string_contains_string(fix_string *str, fix_string *substr)
{
	return fix_string_find_string_from_left(str, substr) != -1;
}

fix_string fix_string_trim_from_left_by_char(fix_string *str, char c)
{
	int ofs = fix_string_find_char_from_right(str, c);
	if(ofs < 0) return *str;
	fix_string new_string = *str;
	new_string.len -= ofs;
	new_string.data += ofs;

	return new_string;
}

fix_string fix_string_trim_from_right_by_char(fix_string *str, char c)
{
	int idx = fix_string_find_char_from_left(str, c);
	if(idx < 0) return *str;
	fix_string new_string = *str;
	new_string.len = idx;

	return new_string;
}

static inline int _char_to_int(char c)
{
	if (c < '0' || c > '9') return 0;
	return (int)(c - '0');
}

int fix_string_to_int(fix_string *str)
{
	int num = 0;
	int start_idx = 0;

	if(str->data[0] == '-') start_idx = 1;

	for (int i = start_idx; i < str->len; ++i)
	{
		int n = _char_to_int(str->data[i]);
		num = (num * 10) + n;
	}

	if (start_idx) num = -num;

	return num;
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