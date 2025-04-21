/* fix_linalg.h
* License: Public Domain or BSD0
* 
* To use this library, just include the header
* All functions are declared static inline
* #include "fix_rect_cut.h"
*
* Implementation of the Rect Cut UI Layouting idea
* https://halt.software/p/rectcut-for-dead-simple-ui-layouts
*
* If you're using fix_linalg in the same project, include it BEFORE fix_rect_cut to avoid conflicts
*/ 

#ifndef FIX_RECT_CUT_H
#define FIX_RECT_CUT_H

#ifndef FIX_LINALG_H
struct fix_rect_s {float x, y, w, h;};
typedef struct fix_rect_s fix_rect;
#endif //FIX_LINALG_RECT

//Cuts out a rect by V from the original's left side. The original is modified.
static inline fix_rect fix_rect_cut_left(fix_rect *rect, float v)
{
	float x = rect->x;
	if(rect->x + v < rect->x + rect->w)
	{
		rect->x = rect->x + v;
		rect->w -= v;
	}

	return (fix_rect){x, rect->y, rect->x - x, rect->h};
}

//Cuts out a rect by a percentage (0.0f - 0.1f) of the parent's width from the parent's left side. The original is modified.
static inline fix_rect fix_rect_cut_left_pct(fix_rect *rect, float pct)
{

	float v = rect->w * pct;
	return fix_rect_cut_left(rect, v);
}

//Cuts out a rect by V from the original's right side. The original is modified.
static inline fix_rect fix_rect_cut_right(fix_rect *rect, float v)
{
	float x = rect->x + rect->w - v;
	if(rect->w - v > rect->x)
	{
		rect->w -= v;
	}

	return (fix_rect){x, rect->y, v, rect->h};
}

//Cuts out a rect by a percentage (0.0f - 0.1f) of the parent's width from the parent's right side. The original is modified.
static inline fix_rect fix_rect_cut_right_pct(fix_rect *rect, float pct)
{

	float v = rect->w * pct;
	return fix_rect_cut_right(rect, v);
}

//Cuts out a rect by V from the original's top side. The original is modified.
static inline fix_rect fix_rect_cut_top(fix_rect *rect, float v)
{
	float y = rect->y;
	if(rect->y + v < rect->y + rect->h)
	{
		rect->y = rect->y + v;
		rect->h -= v;
	}

	return (fix_rect){rect->x, y, rect->w, rect->y - y};
}

//Cuts out a rect by a percentage (0.0f - 0.1f) of the parent's height from the parent's right side. The original is modified.
static inline fix_rect fix_rect_cut_top_pct(fix_rect *rect, float pct)
{

	float v = rect->h * pct;
	return fix_rect_cut_top(rect, v);
}

//Cuts out a rect by V from the original's bottom side. The original is modified.
static inline fix_rect fix_rect_cut_bottom(fix_rect *rect, float v)
{
	float y = rect->y + rect->h - v;
	if(rect->h - v > rect->y)
	{
		rect->h -= v;
	}

	return (fix_rect){rect->x, y, rect->w, v};
}

//Cuts out a rect by a percentage (0.0f - 0.1f) of the parent's height from the parent's bottom side. The original is modified.
static inline fix_rect fix_rect_cut_bottom_pct(fix_rect *rect, float pct)
{

	float v = rect->h * pct;
	return fix_rect_cut_bottom(rect, v);
}

#endif //FIX_RECT_CUT_H

