/* fix_linalg.h
* License: Public Domain or zlib
* 
* To use this library, just include the header
* All functions are declared static inline
* #include "fix_linalg.h"
*
* Linear Algebra structs and functions (plus a bit more)
* This is mainly oriented towards gamedev
*
* TODO
* - mat2x2
* - mat3x3
* - mat4x4
* - quaternions
*/ 

#ifndef FIX_LINALG_H
#define FIX_LINALG_H

#ifdef _cplusplus
extern "C" {
#endif //_cplusplus


#ifndef FIX_LINALG_CUSTOM_MATH
#include <math.h>
#define fix_fabsf fabsf
#define fix_sqrtf sqrtf
#define fix_sinf sinf
#define fix_cosf cosf

#define fix_PIf M_PIf
#endif

// Vec2
union fix_vec2_u 
{
	struct {float x, y;};
	struct {float w, h;};
	struct {float u, v;};
	struct {float s, t;};
	float axes[2];
};
typedef union fix_vec2_u fix_vec2;

static inline fix_vec2 fix_vec2_add(fix_vec2 lhs, fix_vec2 rhs)
{
	return (fix_vec2)
	{{
		lhs.x + rhs.x,
		lhs.y + rhs.y
	}};
}

static inline fix_vec2 fix_vec2_sub(fix_vec2 lhs, fix_vec2 rhs)
{
	return (fix_vec2)
	{{
		lhs.x - rhs.x,
		lhs.y - rhs.y
	}};
}

static inline fix_vec2 fix_vec2_mul(fix_vec2 lhs, fix_vec2 rhs)
{
	return (fix_vec2)
	{{
		lhs.x * rhs.x,
		lhs.y * rhs.y
	}};
}

static inline fix_vec2 fix_vec2_mulf(fix_vec2 lhs, float rhs)
{
	return (fix_vec2)
	{{
		lhs.x * rhs,
		lhs.y * rhs
	}};
}
#define fix_vec2_scale fix_vec2_mulf

static inline fix_vec2 fix_vec2_div(fix_vec2 lhs, fix_vec2 rhs)
{
	return (fix_vec2)
	{{
		lhs.x / rhs.x,
		lhs.y / rhs.y
	}};
}

static inline float fix_vec2_dot(fix_vec2 lhs, fix_vec2 rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

static inline fix_vec2 fix_vec2_normalise(fix_vec2 vec)
{
	return fix_vec2_mulf(vec, 1.0f / fix_sqrtf(vec.x * vec.x + vec.y * vec.y));
}

static inline float fix_vec2_mag(fix_vec2 vec)
{
	return fix_sqrtf(vec.x * vec.x + vec.y * vec.y);
}
#define fix_vec2_len fix_vec2_mag

static inline float fix_vec2_distance(fix_vec2 lhs, fix_vec2 rhs)
{
	return fix_vec2_mag(fix_vec2_sub(rhs, lhs));
}

static inline fix_vec2 fix_vec2_rot(fix_vec2 vec, float angle)
{
	float sin_angle = fix_sinf(angle);
	float cos_angle = fix_cosf(angle);

	return (fix_vec2)
	{{
		vec.x * cos_angle - vec.y * sin_angle,
		vec.x * sin_angle + vec.y * cos_angle
	}};
}

// Vec3
union fix_vec3_u 
{
	struct {float x, y, z;};
	struct {float u, v, w;};
	struct {float r, g, b;};

	struct {fix_vec2 xy; float _z;};
	struct {float _x; fix_vec2 yz;};

	struct {fix_vec2 uv; float _w;};
	struct {float _u; fix_vec2 vw;};

	struct {fix_vec2 rg; float _b;};
	struct {float _r; fix_vec2 gb;};

	float axes[3];
};
typedef union fix_vec3_u fix_vec3;
typedef union fix_vec3_u fix_rgb;

static inline fix_vec3 fix_vec3_add(fix_vec3 lhs, fix_vec3 rhs)
{
	return (fix_vec3)
	{{
		lhs.x + rhs.x,
		lhs.y + rhs.y,
		lhs.z + rhs.z
	}};
}

static inline fix_vec3 fix_vec3_sub(fix_vec3 lhs, fix_vec3 rhs)
{
	return (fix_vec3)
	{{
		lhs.x - rhs.x,
		lhs.y - rhs.y,
		lhs.z - rhs.z
	}};
}

static inline fix_vec3 fix_vec3_mul(fix_vec3 lhs, fix_vec3 rhs)
{
	return (fix_vec3)
	{{
		lhs.x * rhs.x,
		lhs.y * rhs.y,
		lhs.z * rhs.z
	}};
}

static inline fix_vec3 fix_vec3_mulf(fix_vec3 lhs, float rhs)
{
	return (fix_vec3)
	{{
		lhs.x * rhs,
		lhs.y * rhs,
		lhs.z * rhs
	}};
}
#define fix_vec3_scale fix_vec3_mulf

static inline fix_vec3 fix_vec3_div(fix_vec3 lhs, fix_vec3 rhs)
{
	return (fix_vec3)
	{{
		lhs.x / rhs.x,
		lhs.y / rhs.y,
		lhs.z / rhs.z
	}};
}

static inline float fix_vec3_dot(fix_vec3 lhs, fix_vec3 rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

static inline fix_vec3 fix_vec3_normalise(fix_vec3 vec)
{
	return fix_vec3_mulf(vec, 1.0f / fix_sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}

static inline float fix_vec3_mag(fix_vec3 vec)
{
	return fix_sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}
#define fix_vec3_len fix_vec3_mag

static inline float fix_vec3_distance(fix_vec3 lhs, fix_vec3 rhs)
{
	return fix_vec3_mag(fix_vec3_sub(rhs, lhs));
}

static inline fix_vec3 fix_vec3_cross(fix_vec3 lhs, fix_vec3 rhs)
{
	return (fix_vec3)
	{{
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.y,
	}};
}

// Vec4
union fix_vec4_u 
{
	struct {float x, y, z, w;};
	struct {float r, g, b, a;};

	struct {fix_vec2 xy; fix_vec2 zw;};
	struct {float _x; fix_vec2 yz; float _w;};
	
	struct {fix_vec3 xyz; float __w;};
	struct {float __x; fix_vec3 yzw;};

	struct {fix_vec3 rgb; float _a;};
	struct {fix_vec2 rg; fix_vec2 ba;};
	struct {float _r; fix_vec2 gb; float __a;};

	float axes[4];
};
typedef union fix_vec4_u fix_vec4;
typedef union fix_vec4_u fix_rgba;

static inline fix_vec4 fix_vec4_add(fix_vec4 lhs, fix_vec4 rhs)
{
	return (fix_vec4)
	{{
		lhs.x + rhs.x,
		lhs.y + rhs.y,
		lhs.z + rhs.z,
		lhs.w + rhs.w
	}};
}

static inline fix_vec4 fix_vec4_sub(fix_vec4 lhs, fix_vec4 rhs)
{
	return (fix_vec4)
	{{
		lhs.x - rhs.x,
		lhs.y - rhs.y,
		lhs.z - rhs.z,
		lhs.w - rhs.w
	}};
}

static inline fix_vec4 fix_vec4_mul(fix_vec4 lhs, fix_vec4 rhs)
{
	return (fix_vec4)
	{{
		lhs.x * rhs.x,
		lhs.y * rhs.y,
		lhs.z * rhs.z,
		lhs.w * rhs.w
	}};
}

static inline fix_vec4 fix_vec4_mulf(fix_vec4 lhs, float rhs)
{
	return (fix_vec4)
	{{
		lhs.x * rhs,
		lhs.y * rhs,
		lhs.z * rhs,
		lhs.w * rhs,
	}};
}
#define fix_vec4_scale fix_vec4_mulf

static inline fix_vec4 fix_vec4_div(fix_vec4 lhs, fix_vec4 rhs)
{
	return (fix_vec4)
	{{
		lhs.x / rhs.x,
		lhs.y / rhs.y,
		lhs.z / rhs.z,
		lhs.w / rhs.w
	}};
}

static inline float fix_vec4_dot(fix_vec4 lhs, fix_vec4 rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

static inline fix_vec4 fix_vec4_normalise(fix_vec4 vec)
{
	return fix_vec4_mulf(vec, 1.0f / fix_sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w));
}

static inline float fix_vec4_mag(fix_vec4 vec)
{
	return fix_sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}
#define fix_vec4_len fix_vec4_mag

static inline float fix_vec4_distance(fix_vec4 lhs, fix_vec4 rhs)
{
	return fix_vec4_mag(fix_vec4_sub(rhs, lhs));
}

// Vec4
union fix_mat4x4_u 
{
	struct {float m1,  m2,  m3,  m4,
				  m5,  m6,  m7,  m8,
				  m9,  m10, m11, m12,
				  m13, m14, m15, m16;};

	fix_vec4 rows[4];
	float fields[4][4];
	float m[4 * 4];
};
typedef union fix_mat4x4_u fix_mat4x4;
typedef union fix_mat4x4_u fix_mat4;

static inline fix_mat4 fix_mat4_add(fix_mat4 lhs, fix_mat4 rhs)
{
	return (fix_mat4)
	{{
		lhs.m[0]  + rhs.m[0],  lhs.m[1]  + rhs.m[1],  lhs.m[2]  + rhs.m[2],  lhs.m[3]  + rhs.m[3],
		lhs.m[4]  + rhs.m[4],  lhs.m[5]  + rhs.m[5],  lhs.m[6]  + rhs.m[6],  lhs.m[7]  + rhs.m[7],
		lhs.m[8]  + rhs.m[8],  lhs.m[9]  + rhs.m[9],  lhs.m[10] + rhs.m[10], lhs.m[11] + rhs.m[11],
		lhs.m[12] + rhs.m[12], lhs.m[13] + rhs.m[13], lhs.m[14] + rhs.m[14], lhs.m[15] + rhs.m[15],
	}};
}

static inline fix_mat4 fix_mat4_sub(fix_mat4 lhs, fix_mat4 rhs)
{
	return (fix_mat4)
	{{
		lhs.m[0]  - rhs.m[0],  lhs.m[1]  - rhs.m[1],  lhs.m[2]  - rhs.m[2],  lhs.m[3]  - rhs.m[3],
		lhs.m[4]  - rhs.m[4],  lhs.m[5]  - rhs.m[5],  lhs.m[6]  - rhs.m[6],  lhs.m[7]  - rhs.m[7],
		lhs.m[8]  - rhs.m[8],  lhs.m[9]  - rhs.m[9],  lhs.m[10] - rhs.m[10], lhs.m[11] - rhs.m[11],
		lhs.m[12] - rhs.m[12], lhs.m[13] - rhs.m[13], lhs.m[14] - rhs.m[14], lhs.m[15] - rhs.m[15],
	}};
}

static inline fix_mat4 fix_mat4_mul(fix_mat4 lhs, fix_mat4 rhs)
{
	return (fix_mat4)
	{{
		lhs.m[0] * rhs.m[0] + lhs.m[1] * rhs.m[4] + lhs.m[2] * rhs.m[8]  + lhs.m[3] * rhs.m[12],
		lhs.m[0] * rhs.m[1] + lhs.m[1] * rhs.m[5] + lhs.m[2] * rhs.m[9]  + lhs.m[3] * rhs.m[13],
		lhs.m[0] * rhs.m[2] + lhs.m[1] * rhs.m[6] + lhs.m[2] * rhs.m[10] + lhs.m[3] * rhs.m[14],
		lhs.m[0] * rhs.m[3] + lhs.m[1] * rhs.m[7] + lhs.m[2] * rhs.m[11] + lhs.m[3] * rhs.m[15],

		lhs.m[4] * rhs.m[0] + lhs.m[5] * rhs.m[4] + lhs.m[6] * rhs.m[8]  + lhs.m[7] * rhs.m[12],
		lhs.m[4] * rhs.m[1] + lhs.m[5] * rhs.m[5] + lhs.m[6] * rhs.m[9]  + lhs.m[7] * rhs.m[13],
		lhs.m[4] * rhs.m[2] + lhs.m[5] * rhs.m[6] + lhs.m[6] * rhs.m[10] + lhs.m[7] * rhs.m[14],
		lhs.m[4] * rhs.m[3] + lhs.m[5] * rhs.m[7] + lhs.m[6] * rhs.m[11] + lhs.m[7] * rhs.m[15],

		lhs.m[8] * rhs.m[0] + lhs.m[9] * rhs.m[4] + lhs.m[10] * rhs.m[8]  + lhs.m[11] * rhs.m[12],
		lhs.m[8] * rhs.m[1] + lhs.m[9] * rhs.m[5] + lhs.m[10] * rhs.m[9]  + lhs.m[11] * rhs.m[13],
		lhs.m[8] * rhs.m[2] + lhs.m[9] * rhs.m[6] + lhs.m[10] * rhs.m[10] + lhs.m[11] * rhs.m[14],
		lhs.m[8] * rhs.m[3] + lhs.m[9] * rhs.m[7] + lhs.m[10] * rhs.m[11] + lhs.m[11] * rhs.m[15],

		lhs.m[12] * rhs.m[0] + lhs.m[13] * rhs.m[4] + lhs.m[14] * rhs.m[8]  + lhs.m[15] * rhs.m[12],
		lhs.m[12] * rhs.m[1] + lhs.m[13] * rhs.m[5] + lhs.m[14] * rhs.m[9]  + lhs.m[15] * rhs.m[13],
		lhs.m[12] * rhs.m[2] + lhs.m[13] * rhs.m[6] + lhs.m[14] * rhs.m[10] + lhs.m[15] * rhs.m[14],
		lhs.m[12] * rhs.m[3] + lhs.m[13] * rhs.m[7] + lhs.m[14] * rhs.m[11] + lhs.m[15] * rhs.m[15],
	}};
}

static inline fix_mat4 fix_mat4_mulf(fix_mat4 lhs, float rhs)
{
	return (fix_mat4)
	{{
		lhs.m[0]  * rhs, lhs.m[1]  * rhs, lhs.m[2]  * rhs, lhs.m[3]  * rhs,
		lhs.m[4]  * rhs, lhs.m[5]  * rhs, lhs.m[6]  * rhs, lhs.m[7]  * rhs,
		lhs.m[8]  * rhs, lhs.m[9]  * rhs, lhs.m[10] * rhs, lhs.m[11] * rhs,
		lhs.m[12] * rhs, lhs.m[13] * rhs, lhs.m[14] * rhs, lhs.m[15] * rhs,
	}};
}
#define fix_mat4_scale fix_mat4_mulf

// Extras

//Rect
union fix_rect_u
{
	struct {float x, y, w, h;};
	struct { fix_vec2 pos; fix_vec2 size;};
};
typedef union fix_rect_u fix_rect;

static inline int fix_rect_aabb_check(fix_rect first, fix_rect second)
{
	return  first.x < second.x +  second.w &&
			first.x + first.w  >  second.x &&
			first.y < second.y +  second.h &&
			first.y + first.h  >  second.y;
}

static inline fix_vec2 rect_centre(fix_rect rect)
{
	return (fix_vec2)
	{{
		(rect.x + rect.x + rect.w) * 0.5f,
		(rect.y + rect.y + rect.h) * 0.5f
	}};
}


#ifdef _cplusplus
}
#endif //_cplusplus

#endif //FIX_LINALG_H
