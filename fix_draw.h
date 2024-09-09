/* fix_draw.h
* License: Public Domain or zlib
* 
* To use this library, remember to define FIX_DRAW_IMPL in ONE C or C++ file:
* #define FIX_DRAW_IMPL
* #include "fix_draw.h"
*
* Software Renderer which can render some primitives and polygons
*
* fix_draw does not allocate memory, you're in charge of providing the framebuffer array
* that you will pass on to `fix_draw_make_screen`
*
* It can be something simple as:
* uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
* fix_screen s = fix_draw_make_screen(pixels, SCREEN_WIDTH, SCREEN_HEIGHT);
*
* TODO:
* 3D
* allow providing custom implementation for abs and pixel data type
* More color conversion utilities
*/

#ifdef _cplusplus
extern "C" {
#endif //_cplusplus

#ifndef FIX_RENDERER_H
#define FIX_RENDERER_H

//Needed for uint32_t, as we pack RGBA data into one int
#include <stdint.h>

struct fix_screen_s
{
	unsigned int width;
	unsigned int height;
	uint32_t *pixels;
};
typedef struct fix_screen_s fix_screen;

static inline uint32_t fix_RGBA2ABGR(uint32_t RGBA)
{
	return ((RGBA >> 24) & 0x000000FF) |
		   ((RGBA <<  8) & 0x00FF0000) |
		   ((RGBA >>  8) & 0x0000FF00) | 
		   ((RGBA << 24) & 0xFF000000) ;
}
#define fix_ABGR2RGBA fix_RGBA2ABGR

static inline uint32_t fix_ARGB2RGBA(uint32_t ARGB)
{
	return ((ARGB & 0x00FF0000) >> 16) |
		   ((ARGB & 0x0000FF00) >>  0) |
		   ((ARGB & 0x000000FF) << 16) |
		   ((ARGB & 0xFF000000) <<  0) ;
}

/* Stores the framebuffer and related data into a re-usable struct */
fix_screen fix_draw_make_screen(uint32_t *pixels, unsigned int width, unsigned int height);
/* Clears the screen using the recieved colour */
void fix_draw_clear(fix_screen screen, uint32_t color);
/* Sets a pixel to the given colour */
void fix_draw_pixel(fix_screen screen, unsigned int x, unsigned int y, uint32_t color);
/* Draws a rectangle with the given colour */
void fix_draw_rect(fix_screen screen, int x, int y, int w, int h, uint32_t color);
/* Draws a circle with the given colour */
void fix_draw_circle(fix_screen screen, int center_x, int center_y, unsigned int radius, uint32_t color);
/* Draws an ellipse with the given colour */
void fix_draw_ellipse(fix_screen screen, int center_x, int center_y, int radius_x, int radius_y, uint32_t color);
/* Draws the triangle with the given colour */
void fix_draw_triangle(fix_screen screen, int first_x, int first_y, int second_x, int second_y, int third_x, int third_y, uint32_t color);
/* Draws the triangle with the given colour. Accepts vertex data packed into an array */
void fix_draw_triangle_arr(fix_screen screen, int data[6], uint32_t color);
/* Draws a Quad. Vertices are expected to be in counter-clockwise order */
void fix_draw_quad(fix_screen screen, int data[8], uint32_t color);
/* Draws a polygon using triangles. Vertices should be arranged as {x0, y0, x1, y1, ... xn, yn} and should be a multiple of 2.
Indices should treat the vertex array as if it's filled with Vec2 types e.g.: value 2 will retrieve x2, y2 pair, and should be a multiple of 3 */
void fix_draw_poly(fix_screen screen, int vertices[], unsigned int indices[], unsigned int index_count, uint32_t color);
/* Draws an image onto the screen. Can scale up or down */
void fix_draw_image(fix_screen screen, int x, int y, int w, int h, fix_screen src);

#ifndef FIX_DRAW_NO_COLORDEFS
//ABGR
#define ABGR_RED     0xFF0000FF
#define ABGR_GREEN   0x00FF00FF
#define ABGR_BLUE    0x0000FFFF
#define ABGR_BLACK   0x000000FF
#define ABGR_WHITE   0xFFFFFFFF
#define ABGR_CLEAR   0x00000000
#define ABGR_YELLOW  0xFFFF00FF
#define ABGR_MAGENTA 0xFF00FFFF
#define ABGR_CYAN	 0x00FFFFFF
//RGBA
#define RGBA_RED     0xFF0000FF
#define RGBA_GREEN   0xFF00FF00
#define RGBA_BLUE    0xFFFF0000
#define RGBA_BLACK   0xFF000000
#define RGBA_WHITE   0xFFFFFFFF
#define RGBA_CLEAR   0x00000000
#define RGBA_YELLOW  0xFF00FFFF
#define RGBA_MAGENTA 0xFFFF00FF
#define RGBA_CYAN	 0xFFFFFF00
//ARGB
#define ARGB_RED     0x0000FFFF
#define ARGB_GREEN   0x00FF00FF
#define ARGB_BLUE    0xFF0000FF
#define ARGB_BLACK   0x000000FF
#define ARGB_WHITE   0xFFFFFFFF
#define ARGB_CLEAR   0x00000000
#define ARGB_YELLOW  0x00FFFFFF
#define ARGB_MAGENTA 0xFF00FFFF
#define ARGB_CYAN	 0xFFFF00FF
//BGRA
#define BGRA_RED 	 0xFFFF0000
#define BGRA_GREEN 	 0xFF00FF00
#define BGRA_BLUE 	 0xFF0000FF
#define BGRA_BLACK 	 0xFF000000
#define BGRA_WHITE 	 0xFFFFFFFF
#define BGRA_YELLOW	 0xFFFFFF00
#define BGRA_MAGENTA 0xFFFF00FF
#define BGRA_CYAN 	 0xFF00FFFF
#endif //FIX_DRAW_NO_COLORDEFS

#endif //FIX_RENDERER_H

#ifdef FIX_DRAW_IMPL
#include <stdlib.h>

fix_screen fix_draw_make_screen(uint32_t *pixels, unsigned int width, unsigned int height)
{
	return (fix_screen)
	{
		.pixels = pixels,
		.width = width,
		.height = height,
	};
}

void fix_draw_clear(fix_screen screen, uint32_t color)
{
	for(unsigned int y = 0; y < screen.height; y += 1)
	{
		for(unsigned int x = 0; x < screen.width; x += 1)
		{
			screen.pixels[x + screen.width * y] = color;
		}
	}
}

//NOTE(Fix): This is so simple, it could likely go into the header and have it be static inline
void fix_draw_pixel(fix_screen screen, unsigned int x, unsigned int y, uint32_t color)
{
	screen.pixels[x + screen.width * y] = color;
}

void fix_draw_rect(fix_screen screen, int x, int y, int w, int h, uint32_t color)
{
	int x0 = x;
	int y0 = y;

	for(int y = y0; y < y0 + h; y += 1)
	{
		if (y < 0) continue; //We can't draw above the screen, but we may enter the screen
		if (y >= (int)screen.height) break; //We can't draw below the screen, and we're not going back
		for(int x = x0; x < x0 + w; x += 1)
		{
			if (x < 0) continue; //We can't draw left of the screen, but we may enter the screen
			if (x >= (int)screen.width) break; //We can't draw right of the screen, and we're not going back

			screen.pixels[x + screen.width * y] = color;
		}
	}
}

void fix_draw_circle(fix_screen screen, int center_x, int center_y, unsigned int radius, uint32_t color)
{
	int rr = radius * radius;

	/* Grab a rect around the circle
	* then check if the point we want to draw is inside the circle's radius
	* if it isn't, continue
	* if it is, draw it
	*/

	for(int y = center_y - (int)radius; y < center_y + (int)radius; y += 1)
	{
		if (y < 0) continue; //We can't draw above the screen
		if (y >= (int)screen.height) break; //We can't draw below the screen
		for(int x = center_x - (int)radius; x < center_x + (int)radius; x += 1)
		{
			if (x < 0) continue; //We can't draw left of the screen
			if (x >= (int)screen.width) break; //We can't draw right of the screen
			int distance_to_x = x - center_x;
			int distance_to_y = y - center_y;

			if(distance_to_x * distance_to_x + distance_to_y * distance_to_y > rr) continue;

			screen.pixels[x + screen.width * y] = color;
		}
	}
}

void fix_draw_ellipse(fix_screen screen, int center_x, int center_y, int radius_x, int radius_y, uint32_t color)
{
	int rxrx = radius_x * radius_x;
	int ryry = radius_y * radius_y;
	int rxrxryry = rxrx * ryry;

	for(int y = center_y - (int)radius_y; y < center_y + (int)radius_y; y += 1)
	{
		if (y < 0) continue; //We can't draw above the screen
		if (y >= (int)screen.height) break; //We can't draw below the screen
		for(int x = center_x - (int)radius_x; x < center_x + (int)radius_x; x += 1)
		{
			if (x < 0) continue; //We can't draw left of the screen
			if (x >= (int)screen.width) break; //We can't draw right of the screen
			int distance_to_x = x - center_x;
			int distance_to_y = y - center_y;

			if(distance_to_x * distance_to_x * ryry + distance_to_y * distance_to_y * rxrx > rxrxryry) continue;

			screen.pixels[x + screen.width * y] = color;
		}
	}
}

void fix_draw_line(fix_screen screen, int start_x, int start_y, int end_x, int end_y, uint32_t color)
{
	int x0 = start_x;
	int y0 = start_y;
	int x1 = end_x;
	int y1 = end_y;
	int distance_x = x1 - x0;
	int distance_y = y1 - y0;

	if(distance_x == 0 && distance_y == 0) return;

	if(abs(distance_x) < abs(distance_y))
	{
		if(y0 > y1)
		{
			int temp = x0;
			x0 = x1;
			x1 = temp;
			temp = y0;
			y0 = y1;
			y1 = temp;
		}

		for(int y = y0; y <= y1; y += 1)
		{
			if(y >= (int)screen.height) return;
			if(y < 0) continue;
			int x = distance_x * (y - y0) / distance_y + x0;
			if(x < 0 || x >= (int)screen.width) continue;
			screen.pixels[x + screen.width * y] = color;
		}
		return;
	}

	if(x0 > x1)
	{
		int temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
	}

	for (int x = x0; x <= x1; ++x) 
	{
		if(x >= (int)screen.width) return;
		if(x < 0) continue;
		int y = distance_y * (x - x0)/ distance_x + y0;
		if(y < 0 || y >= (int)screen.height) continue;
		screen.pixels[x + screen.width * y] = color;
	}
}

void fix_draw_triangle(fix_screen screen, int first_x, int first_y, int second_x, int second_y, int third_x, int third_y, uint32_t color)
{
	int x0 = first_x;
	int y0 = first_y;
	int x1 = second_x;
	int y1 = second_y;
	int x2 = third_x;
	int y2 = third_y;

	//Note(Fix): Might not even need to do this
	//Sort by Y, so we can leisurly draw top to bottom
	if(y0 > y1)
	{
		int temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
	}

	if(y1 > y2)
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	if(y0 > y1)
	{
		int temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
	}

	/* Grab a rect around the triangle
	* then Check if the point we want to draw is inside the triangle
	* if it isn't, continue
	* if it is, draw it
	*
	* This is the simplest way I could think of
	*/

	int box_x = x0;
	int box_y = y0; //this is for sure, due to sorting above

	if(box_x > x1) box_x = x1;
	if(box_x > x2) box_x = x2;

	int bottom_right_x = x2;
	int bottom_right_y = y2; //this is for sure, due to sorting above

	if(bottom_right_x < x1) bottom_right_x = x1;
	if(bottom_right_x < x0) bottom_right_x = x0;

	int box_w = bottom_right_x - box_x;
	int box_h = bottom_right_y - box_y;

	int triangle_area = abs((x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1)) / 2);

	for(int y = box_y; y < box_y + box_h; y += 1)
	{
		if (y < 0) continue; //We can't draw above the screen
		if (y >= (int)screen.height) break; //We can't draw below the screen
		for(int x = box_x; x < box_x + box_w; x += 1)
		{
			if (x < 0) continue; //We can't draw left of the screen
			if (x >= (int)screen.width) break; //We can't draw right of the screen

			int pbc_area = abs((x * (y1 - y2) + x1 * (y2 - y) + x2 * (y - y1)) / 2);
			int pac_area = abs((x0 * (y - y2) + x * (y2 - y0) + x2 * (y0 - y)) / 2);
			int pab_area = abs((x0 * (y1 - y) + x1 * (y - y0) + x * (y0 - y1)) / 2);

			if(triangle_area != pbc_area + pac_area + pab_area) continue;

			screen.pixels[x + screen.width * y] = color;
		}
	}
}

//NOTE(Fix): This is so simple, it could likely go into the header and have it be static inline
void fix_draw_triangle_arr(fix_screen screen, int data[6], uint32_t color)
{
	fix_draw_triangle(screen, data[0], data[1], data[2], data[3], data[4], data[5], color);
}

void fix_draw_quad(fix_screen screen, int data[8], uint32_t color)
{

	fix_draw_triangle(screen, data[0], data[1], data[2], data[3], data[4], data[5], color);
	fix_draw_triangle(screen, data[4], data[5], data[6], data[7], data[0], data[1], color);
}

void fix_draw_poly(fix_screen screen, int vertices[], unsigned int indices[], unsigned int index_count, uint32_t color)
{
	for(unsigned int i = 0; i < index_count; i += 3)
	{
		fix_draw_triangle(screen, 
			vertices[indices[i]],		  vertices[indices[i] + 1], 
			vertices[indices[i + 1] * 2], vertices[indices[i + 1] * 2 + 1], 
			vertices[indices[i + 2] * 2], vertices[indices[i + 2] * 2 + 1], 
			color);
	}
}

void fix_draw_image(fix_screen screen, int x, int y, int w, int h, fix_screen src)
{
	int x0 = x;
	int y0 = y;

	for(int y = y0; y < y0 + h; y += 1)
	{
		if (y < 0) continue;
		if (y >= (int)screen.height) break; 
		int src_y = (y - y0) * src.height / h;
		for(int x = x0; x < x0 + w; x += 1)
		{
			if (x < 0) continue; 
			if (x >= (int)screen.width) break; 

			int src_x = (x - x0) * src.width / w;

			if(src.pixels[src_x + src.width * src_y] == 0) continue; //HACK(Fix): This is a quick Hack-around to skip blank pixels, todo blending
			screen.pixels[x + screen.width * y] = src.pixels[src_x + src.width * src_y];
		}
	}
}

#endif //FIX_DRAW_IMPL


#ifdef _cplusplus
}
#endif //_cplusplus
