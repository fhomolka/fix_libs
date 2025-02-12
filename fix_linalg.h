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
#define fix_roundf roundf

#define fix_sign(n) ((n > 0) - (n < 0))
#define fix_signf(n) (float)((n > 0.0f) - (n < 0.0f))

#ifdef __USE_GNU
#define fix_PIf M_PIf
#define fix_PI_2f M_PI_2f
#define fix_SQRT2f M_SQRT2f
#define fix_SQRT1_2f M_SQRT1_2f
#else //__USE_GNU
#define fix_PIf 3.14159265358979323846f
#define fix_SQRT2f 1.41421356237309504880f
#define fix_SQRT1_2f 0.70710678118654752440f
#define fix_PI_2f 1.57079632679489661923f
#endif //__USE_GNU
#endif //FIX_LINALG_CUSTOM_MATH

// Extra
// DEG-RAD conversion
static inline float fix_deg2rad(float degrees)
{
	return degrees * (fix_PIf / 180.0f);
}

static inline float fix_rad2deg(float radians)
{
	return radians * (180.0f / fix_PIf);
}

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

static inline fix_vec2 fix_vec2_sign(fix_vec2 vec)
{
	return (fix_vec2){{fix_signf(vec.x), fix_signf(vec.y)}};
}

static inline fix_vec2 fix_vec2_round(fix_vec2 vec)
{
	return (fix_vec2){{fix_roundf(vec.x), fix_roundf(vec.y)}};
}

// vec has to be normalized
static inline fix_vec2 fix_vec2_snap45(fix_vec2 vec)
{
	//Thanks to Inigo Quilez
	//https://twitter.com/iquilezles/status/1879398098436538614
	fix_vec2 w = fix_vec2_round((fix_vec2){{vec.x * fix_SQRT2f, vec.y * fix_SQRT2f}});
	if (fix_fabsf(vec.x) + fix_fabsf(vec.y) > 1.5)
	{
		return (fix_vec2){{w.x * fix_SQRT1_2f, w.y * fix_SQRT1_2f}};
	}

	return w;
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

// Mat2
union fix_mat2x2_u
{
	struct {float m1,  m2,  
				  m3,  m4;};

	fix_vec2 columns[2];
	float fields[2][2];
	float m[2 * 2];
};
typedef union fix_mat2x2_u fix_mat2x2;
typedef union fix_mat2x2_u fix_mat2;

static inline fix_mat2 fix_mat2_add(fix_mat2 lhs, fix_mat2 rhs)
{
	return (fix_mat2)
	{{
		lhs.m[0] + rhs.m[0], lhs.m[1] + rhs.m[1],  
		lhs.m[2] + rhs.m[2], lhs.m[3] + rhs.m[3],
	}};
}

static inline fix_mat2 fix_mat2_sub(fix_mat2 lhs, fix_mat2 rhs)
{
	return (fix_mat2)
	{{
		lhs.m[0] - rhs.m[0], lhs.m[1] - rhs.m[1],  
		lhs.m[2] - rhs.m[2], lhs.m[3] - rhs.m[3],
	}};
}

static inline fix_mat2 fix_mat2_mul(fix_mat2 lhs, fix_mat2 rhs)
{
	return (fix_mat2)
	{{
		lhs.m[0] * rhs.m[0] + lhs.m[1] * rhs.m[2],
		lhs.m[0] * rhs.m[1] + lhs.m[1] * rhs.m[3],

		lhs.m[2] * rhs.m[0] + lhs.m[3] * rhs.m[2],
		lhs.m[2] * rhs.m[1] + lhs.m[3] * rhs.m[3],
	}};
}

static inline fix_mat2 fix_mat2_mulf(fix_mat2 lhs, float rhs)
{
	return (fix_mat2)
	{{
		lhs.m[0] * rhs, lhs.m[1] * rhs,  
		lhs.m[2] * rhs, lhs.m[3] * rhs,
	}};
}
#define fix_mat2_scale fix_mat2_mulf

// Mat3
union fix_mat3x3_u 
{
	struct {float m1,  m2,  m3,  
				  m4,  m5,  m6,  
				  m7,  m8,  m9;};

	fix_vec3 columns[3];
	float fields[3][3];
	float m[3 * 3];
};
typedef union fix_mat3x3_u fix_mat3x3;
typedef union fix_mat3x3_u fix_mat3;

static inline fix_mat3 fix_mat3_add(fix_mat3 lhs, fix_mat3 rhs)
{
	return (fix_mat3)
	{{
		lhs.m[0] + rhs.m[0], lhs.m[1] + rhs.m[1], lhs.m[2] + rhs.m[2],
		lhs.m[3] + rhs.m[3], lhs.m[4] + rhs.m[4], lhs.m[5] + rhs.m[5],
		lhs.m[6] + rhs.m[6], lhs.m[7] + rhs.m[7], lhs.m[8] + rhs.m[8],
	}};
}

static inline fix_mat3 fix_mat3_sub(fix_mat3 lhs, fix_mat3 rhs)
{
	return (fix_mat3)
	{{
		lhs.m[0] - rhs.m[0], lhs.m[1] - rhs.m[1], lhs.m[2] - rhs.m[2],
		lhs.m[3] - rhs.m[3], lhs.m[4] - rhs.m[4], lhs.m[5] - rhs.m[5],
		lhs.m[6] - rhs.m[6], lhs.m[7] - rhs.m[7], lhs.m[8] - rhs.m[8],
	}};
}

static inline fix_mat3 fix_mat3_mul(fix_mat3 lhs, fix_mat3 rhs)
{
	return (fix_mat3)
	{{
		lhs.m[0] * rhs.m[0] + lhs.m[1] * rhs.m[3] + lhs.m[2] * rhs.m[6],
		lhs.m[0] * rhs.m[1] + lhs.m[1] * rhs.m[4] + lhs.m[2] * rhs.m[7],
		lhs.m[0] * rhs.m[2] + lhs.m[1] * rhs.m[5] + lhs.m[2] * rhs.m[8],

		lhs.m[3] * rhs.m[0] + lhs.m[4] * rhs.m[3] + lhs.m[5] * rhs.m[6],
		lhs.m[3] * rhs.m[1] + lhs.m[4] * rhs.m[4] + lhs.m[5] * rhs.m[7],
		lhs.m[3] * rhs.m[2] + lhs.m[4] * rhs.m[5] + lhs.m[5] * rhs.m[8],

		lhs.m[6] * rhs.m[0] + lhs.m[7] * rhs.m[3] + lhs.m[8] * rhs.m[6],
		lhs.m[6] * rhs.m[1] + lhs.m[7] * rhs.m[4] + lhs.m[8] * rhs.m[7],
		lhs.m[6] * rhs.m[2] + lhs.m[7] * rhs.m[5] + lhs.m[8] * rhs.m[8],
	}};
}

static inline fix_mat3 fix_mat3_mulf(fix_mat3 lhs, float rhs)
{
	return (fix_mat3)
	{{
		lhs.m[0] * rhs,  lhs.m[1] * rhs,  lhs.m[2] * rhs,
		lhs.m[3] * rhs,  lhs.m[4] * rhs,  lhs.m[5] * rhs,
		lhs.m[6] * rhs,  lhs.m[7] * rhs,  lhs.m[8] * rhs,
	}};
}
#define fix_mat3_scale fix_mat3_mulf

// Mat4
union fix_mat4x4_u 
{
	struct {float m1,  m2,  m3,  m4,
				  m5,  m6,  m7,  m8,
				  m9,  m10, m11, m12,
				  m13, m14, m15, m16;};

	fix_vec4 columns[4];
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

static inline fix_mat4 fix_mat4_ortho(float left, float right, float bottom, float top, float near, float far)
{
	return (fix_mat4)
	{{
		2.0f / (right - left), 0.0f,				  0.0f,				   -(right + left) / (right - left),
		0.0f,				   2.0f / (top - bottom), 0.0f,				   -(top + bottom) / (top - bottom),
		0.0f,				   0.0f,				 -2.0f / (far - near), -(far + near) / (far - near),
		0.0f,				   0.0f,				  0.0f,					1.0f,
	}};
}

static inline fix_mat4 fix_mat4_perspective(float fov_y, float aspect, float near, float far)
{
	float f = 1.0f / tanf(fov_y / 2.0f);
	return (fix_mat4)
	{{
		f / aspect,	0.0f, 0.0f,							0.0f,
		0.0f,		f,	  0.0f,							0.0f,
		0.0f,		0.0f, (near + far) / (near - far), -2.0f * ((near * far) / (near - far)),
		0.0f,		0.0f, -1.0f, 						0.0f,
	}};
}

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

// Color Conversion
#include <stdint.h>

static inline uint32_t fix_rgba2rgba32(fix_rgba c)
{
	int r = c.r * 255;
	int g = c.g * 255;
	int b = c.b * 255;
	int a = c.a * 255;

	return a + (b >> 8) + (g >> 16) + (r >> 24);
}

static inline uint32_t fix_rgba2abgr32(fix_rgba c)
{
	int r = c.r * 255;
	int g = c.g * 255;
	int b = c.b * 255;
	int a = c.a * 255;

	return r + (g >> 8) + (b >> 16) + (a >> 24);
}

#ifdef _cplusplus
}
#endif //_cplusplus

#endif //FIX_LINALG_H
