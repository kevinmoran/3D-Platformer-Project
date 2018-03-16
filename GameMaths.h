#pragma once
// Kevin's game maths library
// Based off simple maths library included with online source for 'Anton's OpenGL Tutorials' book by Anton Gerdelan
// License: https://github.com/capnramses/antons_opengl_tutorials_book/blob/master/LICENCE.md
// Modified and added to by me over the years
// TODO: Maybe SIMD if it'll help, never needed it so far

//Original header:
/******************************************************************************\
| Anton's Maths Library                                                        |
| Email: anton at antongerdelan dot net                                        |
| Revised and inlined into a header file: 16 Jun 2014                          |
| Copyright Dr Anton Gerdelan                                                  |
|******************************************************************************|
| Commonly-used maths structures and functions                                 |
| Simple-as-possible. No disgusting templates.                                 |
| Structs vec3, mat4, versor. just hold arrays of floats called "v","m","q",   |
| respectively. So, for example, to get values from a mat4 do: my_mat.m        |
| A versor is the proper name for a unit quaternion.                           |
| This is C++ because it's sort-of convenient to be able to use maths operators|
\******************************************************************************/

#include <stdio.h>
#include <math.h>

#define PI32 3.14159265359f
#define PI64 3.1415926535897931

//------------------------------------------------------------------------------
// Useful functions
//------------------------------------------------------------------------------
#define MIN(a,b) (((a)<(b)) ? (a) : (b))
#define MAX(a,b) (((a)>(b)) ? (a) : (b))
#define CLAMP(x,lo,hi) (MIN ((hi), MAX ((lo), (x))))

//Compare two floats for equality
inline bool almost_equal(float a, float b, float eps = 0.000001f) {
	return (fabs(a-b) < eps);
}

inline float DEG2RAD(float degs) {
	return degs * (PI32 / 180.0f);
}
inline float RAD2DEG(float rads) {
	return rads * (180.0f / PI32);
}

//------------------------------------------------------------------------------
// Library contents / forward declarations
//------------------------------------------------------------------------------
// data structures
struct vec2;
struct vec3;
struct vec4;
struct mat3;
struct mat4;
struct versor;

// vector functions
inline float length(vec2 v);
inline float length2(vec2 v);
inline vec2 normalise(vec2 v);
inline float dot(vec2 a, vec2 b);

inline float length(vec3 v);
inline float length2(vec3 v);
inline vec3 normalise(vec3 v);
inline float dot(vec3 a, vec3 b);
inline vec3 cross(vec3 a, vec3 b);

// matrix functions
// inline mat3 identity_mat3();
inline mat4 identity_mat4();
inline float determinant(mat4 mm);
inline mat4 inverse(mat4 mm);
inline mat4 transpose(mat4 mm);

// affine functions
inline mat4 translate(mat4 m, vec3 v);
inline mat4 rotate_x_deg(mat4 m, float deg);
inline mat4 rotate_x_deg_mat4(float deg);
inline mat4 rotate_y_deg(mat4 m, float deg);
inline mat4 rotate_y_deg_mat4(float deg);
inline mat4 rotate_z_deg(mat4 m, float deg);
inline mat4 rotate_z_deg_mat4(float deg);
inline mat4 rotate_axis_deg_mat4(vec3 u, float deg);
inline mat4 rotate_align_mat4(vec3 u1, vec3 u2);
inline mat4 scale(mat4 m, vec3 v);
inline mat4 scale_mat4(vec3 v);
inline mat4 scale(mat4 m, float s);

// camera functions
inline mat4 look_at(vec3 cam_pos, vec3 target_pos, vec3 up);
inline mat4 orthographic(float left, float right, float bottom, float top, float near_z, float far_z);
inline mat4 perspective(float fov, float aspect_ratio, float near_z, float far_z);

// quaternion functions
inline versor quat_from_axis_rad(float radians, float x, float y, float z);
inline versor quat_from_axis_deg(float degrees, float x, float y, float z);
inline versor quat_from_axis_deg(float degrees, vec3 a);
inline mat4 quat_to_mat4(versor q);
inline float dot(versor q, versor r);
inline versor slerp(versor q, versor r);
inline versor normalise(versor q);
inline float dot(versor q, versor r);
inline versor slerp(versor& q, versor& r, float t);

// print functions
inline void print(vec2 v);
inline void print(vec3 v);
inline void print(vec4 v);
inline void print(mat3 m);
inline void print(mat4 m);
inline void print(versor q);

//------------------------------------------------------------------------------
//Suppress anonymous struct warning
#ifdef __GNUC__
#pragma GCC diagnostic push
//This is awful but ignoring "-Wgnu-anonymous-struct" doesn't work on MinGW :(
#pragma GCC diagnostic ignored "-Wpedantic" 
#endif

#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
#endif

#ifdef _MSC_VER
#pragma warning(push, disable: 4201)
#endif
//------------------------------------------------------------------------------
// structs
//------------------------------------------------------------------------------

struct vec2 {
	union {
		struct{
			float x, y;
		};
		float v[2];
	};
};

struct vec3 {
	union {
		struct {
			float x, y, z;
		};
		struct {
			vec2 xy;
			float _IGNOREDz;
		};
		float v[3];
	};
};

struct vec4 {
	union{
		struct{
			float x, y, z, w;
		};
		struct {
			vec3 xyz;
			float _IGNOREDw;
		};
		struct{
			float r, g, b, a;
		};
		float v[4];
	};
};

struct mat3 {
	float m[9];
	/* note: entered in COLUMNS. Stored like this:
		0 3 6
		1 4 7
		2 5 8 
	*/
};

struct mat4 {
	float m[16];
	/* stored like this:
		0  4  8 12
		1  5  9 13
		2  6 10 14
		3  7 11 15
	*/
};

struct versor {
	float q[4];
};

//------------------------------------------------------------------------------
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef __clang__
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

//------------------------------------------------------------------------------
// Operator Overloads
//------------------------------------------------------------------------------

// vec2
inline vec2 operator+ (vec2 lhs, vec2 rhs) {
	vec2 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	return result;
}
inline vec2 operator- (vec2 lhs, vec2 rhs) {
	vec2 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	return result;
}
inline vec2 operator* (vec2 lhs, float rhs) {
	vec2 result;
	result.x = lhs.x * rhs;
	result.y = lhs.y * rhs;
	return result;
}
inline vec2 operator* (float lhs, vec2 rhs) {
	vec2 result;
	result.x = rhs.x * lhs;
	result.y = rhs.y * lhs;
	return result;
}
inline vec2 operator/ (vec2 lhs, float rhs) {
	vec2 result;
	result.x = lhs.x / rhs;
	result.y = lhs.y / rhs;
	return result;
}
inline vec2 operator+= (vec2& lhs, vec2 rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}
inline vec2 operator-= (vec2& lhs, vec2 rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}
inline vec2 operator*= (vec2& lhs, float rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}
inline vec2 operator- (vec2 rhs) {
	vec2 result;
	result.x = -rhs.x;
	result.y = -rhs.y;
	return result;
}
inline bool operator== (vec2 lhs, vec2 rhs) {
	bool result = (almost_equal(lhs.x, rhs.x) && almost_equal(lhs.y, rhs.y));
	return result;
}

// vec3
inline vec3 operator+ (vec3 lhs, vec3 rhs) {
	vec3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}
inline vec3 operator- (vec3 lhs, vec3 rhs) {
	vec3 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	return result;
}
inline vec3 operator* (vec3 lhs, float rhs) {
	vec3 result;
	result.x = lhs.x * rhs;
	result.y = lhs.y * rhs;
	result.z = lhs.z * rhs;
	return result;
}
inline vec3 operator* (float lhs, vec3 rhs) {
	vec3 result;
	result.x = rhs.x * lhs;
	result.y = rhs.y * lhs;
	result.z = rhs.z * lhs;
	return result;
}
inline vec3 operator/ (vec3 lhs, float rhs) {
	vec3 result;
	result.x = lhs.x / rhs;
	result.y = lhs.y / rhs;
	result.z = lhs.z / rhs;
	return result;
}
inline vec3 operator+= (vec3& lhs, vec3 rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs;
}
inline vec3 operator-= (vec3& lhs, vec3 rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	return lhs;
}
inline vec3 operator*= (vec3& lhs, float rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	lhs.z *= rhs;
	return lhs;
}
inline vec3 operator- (vec3 rhs) {
	vec3 result;
	result.x = -rhs.x;
	result.y = -rhs.y;
	result.z = -rhs.z;
	return result;
}
inline bool operator== (vec3 lhs, vec3 rhs) {
	bool result = (almost_equal(lhs.x, rhs.x) && almost_equal(lhs.y, rhs.y) && almost_equal(lhs.z, rhs.z));
	return result;
}

//vec4
inline vec4 operator+ (vec4 lhs, vec4 rhs) {
	vec4 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	result.w = lhs.w + rhs.w;
	return result;
}
inline vec4 operator- (vec4 lhs, vec4 rhs) {
	vec4 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	result.w = lhs.w - rhs.w;
	return result;
}
inline vec4 operator* (vec4 lhs, float rhs) {
	vec4 result;
	result.x = lhs.x * rhs;
	result.y = lhs.y * rhs;
	result.z = lhs.z * rhs;
	result.w = lhs.w * rhs;
	return result;
}
inline vec4 operator* (float lhs, vec4 rhs) {
	vec4 result;
	result.x = rhs.x * lhs;
	result.y = rhs.y * lhs;
	result.z = rhs.z * lhs;
	result.w = rhs.w * lhs;
	return result;
}

// mat3
inline vec3 operator* (mat3 lhs, vec3 rhs) {
	vec3 result;
	// 0x + 3y + 6z
	result.x =
		lhs.m[0] * rhs.x +
		lhs.m[3] * rhs.y +
		lhs.m[6] * rhs.z;
	// 1x + 4y + 7z
	result.y = 
		lhs.m[1] * rhs.x +
		lhs.m[4] * rhs.y +
		lhs.m[7] * rhs.z;
	// 2x + 5y + 8z
	result.z = 
		lhs.m[2] * rhs.x +
		lhs.m[5] * rhs.y +
		lhs.m[8] * rhs.z;
	return result;
}

//mat4
inline vec4 operator* (mat4 lhs, vec4 rhs) {
	vec4 result;
	// 0x + 4y + 8z + 12w
	result.x =
		lhs.m[0] * rhs.x +
		lhs.m[4] * rhs.y +
		lhs.m[8] * rhs.z +
		lhs.m[12] * rhs.w;
	// 1x + 5y + 9z + 13w
	result.y = 
		lhs.m[1] * rhs.x +
		lhs.m[5] * rhs.y +
		lhs.m[9] * rhs.z +
		lhs.m[13] * rhs.w;
	// 2x + 6y + 10z + 14w
	result.z = 
		lhs.m[2] * rhs.x +
		lhs.m[6] * rhs.y +
		lhs.m[10] * rhs.z +
		lhs.m[14] * rhs.w;
	// 3x + 7y + 11z + 15w
	result.w = 
		lhs.m[3] * rhs.x +
		lhs.m[7] * rhs.y +
		lhs.m[11] * rhs.z +
		lhs.m[15] * rhs.w;
	return result;
}
inline mat4 operator* (mat4 lhs, mat4 rhs) {
	mat4 result = {};
	int r_index = 0;
	for(int col = 0; col < 4; ++col) {
		for(int row = 0; row < 4; ++row) {
			float sum = 0.0f;
			for(int i = 0; i < 4; ++i) {
				sum += rhs.m[i + col * 4] * lhs.m[row + i * 4];
			}
			result.m[r_index++] = sum;
		}
	}
	return result;
}

// versor
inline versor operator* (versor lhs, float rhs) {
	versor result;
	result.q[0] = lhs.q[0] * rhs;
	result.q[1] = lhs.q[1] * rhs;
	result.q[2] = lhs.q[2] * rhs;
	result.q[3] = lhs.q[3] * rhs;
	return result;
}
inline versor operator* (float lhs, versor rhs) {
	versor result;
	result.q[0] = rhs.q[0] * lhs;
	result.q[1] = rhs.q[1] * lhs;
	result.q[2] = rhs.q[2] * lhs;
	result.q[3] = rhs.q[3] * lhs;
	return result;
}
inline versor operator* (versor lhs, versor rhs) {
	versor result;
	result.q[0] = rhs.q[0] * lhs.q[0] - rhs.q[1] * lhs.q[1] -
		rhs.q[2] * lhs.q[2] - rhs.q[3] * lhs.q[3];
	result.q[1] = rhs.q[0] * lhs.q[1] + rhs.q[1] * lhs.q[0] -
		rhs.q[2] * lhs.q[3] + rhs.q[3] * lhs.q[2];
	result.q[2] = rhs.q[0] * lhs.q[2] + rhs.q[1] * lhs.q[3] +
		rhs.q[2] * lhs.q[0] - rhs.q[3] * lhs.q[1];
	result.q[3] = rhs.q[0] * lhs.q[3] - rhs.q[1] * lhs.q[2] +
		rhs.q[2] * lhs.q[1] + rhs.q[3] * lhs.q[0];
	// re-normalise in case of mangling
	return normalise(result);
}
inline versor operator/ (versor lhs, float rhs) {
	versor result;
	result.q[0] = lhs.q[0] / rhs;
	result.q[1] = lhs.q[1] / rhs;
	result.q[2] = lhs.q[2] / rhs;
	result.q[3] = lhs.q[3] / rhs;
	return result;
}
inline versor operator+ (versor lhs, versor rhs) {
	versor result;
	result.q[0] = lhs.q[0] + rhs.q[0];
	result.q[1] = lhs.q[1] + rhs.q[1];
	result.q[2] = lhs.q[2] + rhs.q[2];
	result.q[3] = lhs.q[3] + rhs.q[3];
	// re-normalise in case of mangling
	return normalise(result);
}
inline versor operator- (versor lhs, versor rhs) {
	versor result;
	result.q[0] = lhs.q[0] - rhs.q[0];
	result.q[1] = lhs.q[1] - rhs.q[1];
	result.q[2] = lhs.q[2] - rhs.q[2];
	result.q[3] = lhs.q[3] - rhs.q[3];
	// re-normalise in case of mangling
	return normalise(result);
}
inline versor operator+= (versor& lhs, versor rhs) {
	lhs = lhs + rhs;
	return lhs;
}
inline versor operator-= (versor& lhs, versor rhs) {
	lhs = lhs - rhs;
	return lhs;
}
inline versor operator*= (versor& lhs, versor rhs) {
	lhs = lhs * rhs;
	return lhs;
}
inline versor operator- (versor rhs) {
	versor result = rhs*(-1);
	return result;
}

/*------------------------------VECTOR FUNCTIONS------------------------------*/
//---vec2---//
inline float length(vec2 v) {
	return sqrtf((v.x * v.x) + (v.y * v.y));
}
inline float length2(vec2 v) {
	return ((v.x * v.x) + (v.y * v.y));
}
inline vec2 normalise(vec2 v) {
	vec2 result = {};
	float len_squared = length2(v);
	if(len_squared != 0.0f) {
		result = v / sqrtf(len_squared);
	}
	return result;
}
inline float dot(vec2 a, vec2 b) {
	return ((a.x * b.x) + (a.y * b.y));
}

//---vec3---//
inline float length(vec3 v) {
	return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}
inline float length2(vec3 v) {
	return ((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}
inline vec3 normalise(vec3 v) {
	vec3 result = {};
	float len_squared = length2(v);
	if(len_squared != 0.0f) {
		result = v / sqrtf(len_squared);
	}
	return result;
}
inline float dot(vec3 a, vec3 b) {
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}
inline vec3 cross(vec3 a, vec3 b) {
	vec3 result;
	result.x = ((a.y * b.z) - (a.z * b.y));
	result.y = ((a.z * b.x) - (a.x * b.z));
	result.z = ((a.x * b.y) - (a.y * b.x));
	return result;
}

/*-----------------------------MATRIX FUNCTIONS-------------------------------*/
/*
inline mat3 identity_mat3() {
	return mat3 {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
}
*/

inline mat4 identity_mat4() {
	return mat4 {
		1.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

inline mat3 mat4_to_mat3(mat4 RTS){
	//Grab R and S from RTS matrix
	mat3 result = {
		RTS.m[0], RTS.m[1], RTS.m[2],
		RTS.m[4], RTS.m[5], RTS.m[6],
		RTS.m[8], RTS.m[9], RTS.m[10]
	};
	return result;
}

// returns a scalar value with the determinant for a 4x4 matrix
// see http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/fourD/index.htm
inline float determinant(mat4 mm) {
	return
		mm.m[12] * mm.m[9] * mm.m[6] * mm.m[3] -
		mm.m[8] * mm.m[13] * mm.m[6] * mm.m[3] -
		mm.m[12] * mm.m[5] * mm.m[10] * mm.m[3] +
		mm.m[4] * mm.m[13] * mm.m[10] * mm.m[3] +
		mm.m[8] * mm.m[5] * mm.m[14] * mm.m[3] -
		mm.m[4] * mm.m[9] * mm.m[14] * mm.m[3] -
		mm.m[12] * mm.m[9] * mm.m[2] * mm.m[7] +
		mm.m[8] * mm.m[13] * mm.m[2] * mm.m[7] +
		mm.m[12] * mm.m[1] * mm.m[10] * mm.m[7] -
		mm.m[0] * mm.m[13] * mm.m[10] * mm.m[7] -
		mm.m[8] * mm.m[1] * mm.m[14] * mm.m[7] +
		mm.m[0] * mm.m[9] * mm.m[14] * mm.m[7] +
		mm.m[12] * mm.m[5] * mm.m[2] * mm.m[11] -
		mm.m[4] * mm.m[13] * mm.m[2] * mm.m[11] -
		mm.m[12] * mm.m[1] * mm.m[6] * mm.m[11] +
		mm.m[0] * mm.m[13] * mm.m[6] * mm.m[11] +
		mm.m[4] * mm.m[1] * mm.m[14] * mm.m[11] -
		mm.m[0] * mm.m[5] * mm.m[14] * mm.m[11] -
		mm.m[8] * mm.m[5] * mm.m[2] * mm.m[15] +
		mm.m[4] * mm.m[9] * mm.m[2] * mm.m[15] +
		mm.m[8] * mm.m[1] * mm.m[6] * mm.m[15] -
		mm.m[0] * mm.m[9] * mm.m[6] * mm.m[15] -
		mm.m[4] * mm.m[1] * mm.m[10] * mm.m[15] +
		mm.m[0] * mm.m[5] * mm.m[10] * mm.m[15];
}

/* returns a 16-element array that is the inverse of a 16-element array (4x4
matrix). see http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm */
inline mat4 inverse(mat4 mm) {
	float det = determinant(mm);
	
	if(det == 0.0f) {
		fprintf(stderr, "WARNING. matrix has no determinant. can not invert\n");
		return mm;
	}
	float inv_det = 1.0f / det;
	
	return mat4 {
		inv_det * (
			(mm.m[9] * mm.m[14] * mm.m[7]) - (mm.m[13] * mm.m[10] * mm.m[7]) +
			(mm.m[13] * mm.m[6] * mm.m[11]) - (mm.m[5] * mm.m[14] * mm.m[11]) -
			(mm.m[9] * mm.m[6] * mm.m[15]) + (mm.m[5] * mm.m[10] * mm.m[15])
		),
		inv_det * (
			(mm.m[13] * mm.m[10] * mm.m[3]) - (mm.m[9] * mm.m[14] * mm.m[3]) -
			(mm.m[13] * mm.m[2] * mm.m[11]) + (mm.m[1] * mm.m[14] * mm.m[11]) +
			(mm.m[9] * mm.m[2] * mm.m[15]) - (mm.m[1] * mm.m[10] * mm.m[15])
		),
		inv_det * (
			(mm.m[5] * mm.m[14] * mm.m[3]) - (mm.m[13] * mm.m[6] * mm.m[3]) +
			(mm.m[13] * mm.m[2] * mm.m[7]) - (mm.m[1] * mm.m[14] * mm.m[7]) -
			(mm.m[5] * mm.m[2] * mm.m[15]) + (mm.m[1] * mm.m[6] * mm.m[15])
		),
		inv_det * (
			(mm.m[9] * mm.m[6] * mm.m[3]) - (mm.m[5] * mm.m[10] * mm.m[3]) -
			(mm.m[9] * mm.m[2] * mm.m[7]) + (mm.m[1] * mm.m[10] * mm.m[7]) +
			(mm.m[5] * mm.m[2] * mm.m[11]) - (mm.m[1] * mm.m[6] * mm.m[11])
		),
		inv_det * (
			(mm.m[12] * mm.m[10] * mm.m[7]) - (mm.m[8] * mm.m[14] * mm.m[7]) -
			(mm.m[12] * mm.m[6] * mm.m[11]) + (mm.m[4] * mm.m[14] * mm.m[11]) +
			(mm.m[8] * mm.m[6] * mm.m[15]) - (mm.m[4] * mm.m[10] * mm.m[15])
		),
		inv_det * (
			(mm.m[8] * mm.m[14] * mm.m[3]) - (mm.m[12] * mm.m[10] * mm.m[3]) +
			(mm.m[12] * mm.m[2] * mm.m[11]) - (mm.m[0] * mm.m[14] * mm.m[11]) -
			(mm.m[8] * mm.m[2] * mm.m[15]) + (mm.m[0] * mm.m[10] * mm.m[15])
		),
		inv_det * (
			(mm.m[12] * mm.m[6] * mm.m[3]) - (mm.m[4] * mm.m[14] * mm.m[3]) -
			(mm.m[12] * mm.m[2] * mm.m[7]) + (mm.m[0] * mm.m[14] * mm.m[7]) +
			(mm.m[4] * mm.m[2] * mm.m[15]) - (mm.m[0] * mm.m[6] * mm.m[15])
		),
		inv_det * (
			(mm.m[4] * mm.m[10] * mm.m[3]) - (mm.m[8] * mm.m[6] * mm.m[3]) +
			(mm.m[8] * mm.m[2] * mm.m[7]) - (mm.m[0] * mm.m[10] * mm.m[7]) -
			(mm.m[4] * mm.m[2] * mm.m[11]) + (mm.m[0] * mm.m[6] * mm.m[11])
		),
		inv_det * (
			(mm.m[8] * mm.m[13] * mm.m[7]) - (mm.m[12] * mm.m[9] * mm.m[7]) +
			(mm.m[12] * mm.m[5] * mm.m[11]) - (mm.m[4] * mm.m[13] * mm.m[11]) -
			(mm.m[8] * mm.m[5] * mm.m[15]) + (mm.m[4] * mm.m[9] * mm.m[15])
		),
		inv_det * (
			(mm.m[12] * mm.m[9] * mm.m[3]) - (mm.m[8] * mm.m[13] * mm.m[3]) -
			(mm.m[12] * mm.m[1] * mm.m[11]) + (mm.m[0] * mm.m[13] * mm.m[11]) +
			(mm.m[8] * mm.m[1] * mm.m[15]) - (mm.m[0] * mm.m[9] * mm.m[15])
		),
		inv_det * (
			(mm.m[4] * mm.m[13] * mm.m[3]) - (mm.m[12] * mm.m[5] * mm.m[3]) +
			(mm.m[12] * mm.m[1] * mm.m[7]) - (mm.m[0] * mm.m[13] * mm.m[7]) -
			(mm.m[4] * mm.m[1] * mm.m[15]) + (mm.m[0] * mm.m[5] * mm.m[15])
		),
		inv_det * (
			(mm.m[8] * mm.m[5] * mm.m[3]) - (mm.m[4] * mm.m[9] * mm.m[3]) -
			(mm.m[8] * mm.m[1] * mm.m[7]) + (mm.m[0] * mm.m[9] * mm.m[7]) +
			(mm.m[4] * mm.m[1] * mm.m[11]) - (mm.m[0] * mm.m[5] * mm.m[11])
		),
		inv_det * (
			(mm.m[12] * mm.m[9] * mm.m[6]) - (mm.m[8] * mm.m[13] * mm.m[6]) -
			(mm.m[12] * mm.m[5] * mm.m[10]) + (mm.m[4] * mm.m[13] * mm.m[10]) +
			(mm.m[8] * mm.m[5] * mm.m[14]) - (mm.m[4] * mm.m[9] * mm.m[14])
		),
		inv_det * (
			(mm.m[8] * mm.m[13] * mm.m[2]) - (mm.m[12] * mm.m[9] * mm.m[2]) +
			(mm.m[12] * mm.m[1] * mm.m[10]) - (mm.m[0] * mm.m[13] * mm.m[10]) -
			(mm.m[8] * mm.m[1] * mm.m[14]) + (mm.m[0] * mm.m[9] * mm.m[14])
		),
		inv_det * (
			(mm.m[12] * mm.m[5] * mm.m[2]) - (mm.m[4] * mm.m[13] * mm.m[2]) -
			(mm.m[12] * mm.m[1] * mm.m[6]) + (mm.m[0] * mm.m[13] * mm.m[6]) +
			(mm.m[4] * mm.m[1] * mm.m[14]) - (mm.m[0] * mm.m[5] * mm.m[14])
		),
		inv_det * (
			(mm.m[4] * mm.m[9] * mm.m[2]) - (mm.m[8] * mm.m[5] * mm.m[2]) +
			(mm.m[8] * mm.m[1] * mm.m[6]) - (mm.m[0] * mm.m[9] * mm.m[6]) -
			(mm.m[4] * mm.m[1] * mm.m[10]) + (mm.m[0] * mm.m[5] * mm.m[10])
		)
	};
}

// returns a 16-element array flipped on the main diagonal
inline mat4 transpose(mat4 mm) {
	return mat4 {
		mm.m[0], mm.m[4], mm.m[8], mm.m[12],
		mm.m[1], mm.m[5], mm.m[9], mm.m[13],
		mm.m[2], mm.m[6], mm.m[10], mm.m[14],
		mm.m[3], mm.m[7], mm.m[11], mm.m[15]
	};
}

/*--------------------------AFFINE MATRIX FUNCTIONS---------------------------*/
// Returns matrix m translated by [x, y, z]
inline mat4 translate(mat4 m, vec3 v) {
	mat4 result = m;
	result.m[12] += v.x;
	result.m[13] += v.y;
	result.m[14] += v.z;
	return result;
}

// Returns matrix m rotated about x-axis by deg degrees
inline mat4 rotate_x_deg(mat4 m, float deg) {
	float rad = DEG2RAD(deg);
	mat4 m_r = identity_mat4();
	float sin_theta = sinf(rad);
	float cos_theta = cosf(rad);
	m_r.m[5] = cos_theta;
	m_r.m[9] = -sin_theta;
	m_r.m[6] = sin_theta;
	m_r.m[10] = cos_theta;
	return m_r * m;
}

// Returns matrix to rotate about x-axis by deg degrees
inline mat4 rotate_x_deg_mat4(float deg) {
	float rad = DEG2RAD(deg);
	mat4 result = identity_mat4();
	float sin_theta = sinf(rad);
	float cos_theta = cosf(rad);
	result.m[5] = cos_theta;
	result.m[9] = -sin_theta;
	result.m[6] = sin_theta;
	result.m[10] = cos_theta;
	return result;
}

// Returns matrix m rotated about y-axis by rad radians
inline mat4 rotate_y_rad(mat4 m, float rad) 
{
	/*  
	  cos_t  0  sin_t  0           a e i m
	      0  1      0  0           b f j n
	 -sin_t  0  cos_t  0           c g k o
	      0  0      0  1           d h l p

	(acos_t + csin_t) (ecos_t + gsin_t) (icos_t + ksin_t) (mcos_t + osin_t)
	b f j n
	(a(-sin_t) + ccos_t) (e(-sin_t) + gcos_t) (i(-sin_t) + kcos_t) (m(-sin_t) + ocos_t)
	d h l p
	*/
	mat4 result = m;
	float sin_theta = sinf(rad);
	float cos_theta = cosf(rad);

	vec4 row0 = {result.m[0], result.m[4], result.m[8], result.m[12]}; // a e i m
	vec4 row2 = {result.m[2], result.m[6], result.m[10], result.m[14]}; // c g k o

	vec4 row0_cos_theta = cos_theta * row0;
	vec4 row2_cos_theta = cos_theta * row2;
	vec4 row0_sin_theta = sin_theta * row0;
	vec4 row2_sin_theta = sin_theta * row2;

	vec4 result_row0 = row0_cos_theta + row2_sin_theta;
	vec4 result_row2 = row2_cos_theta - row0_sin_theta;

	result.m[0]  = result_row0.x;
	result.m[4]  = result_row0.y;
	result.m[8]  = result_row0.z;
	result.m[12] = result_row0.w;

	result.m[2]  = result_row2.x;
	result.m[6]  = result_row2.y;
	result.m[10] = result_row2.z;
	result.m[14] = result_row2.w;

	return result;
}

// Returns matrix m rotated about y-axis by deg degrees
inline mat4 rotate_y_deg(mat4 m, float deg) 
{
	float rad = DEG2RAD(deg);
	mat4 result = rotate_y_rad(m, rad);
	return result;
}

// Returns matrix to rotate about y-axis by deg degrees
inline mat4 rotate_y_deg_mat4(float deg) {
	float rad = DEG2RAD(deg);
	mat4 result = identity_mat4();
	float sin_theta = sinf(rad);
	float cos_theta = cosf(rad);
	result.m[0] = cos_theta;
	result.m[8] = sin_theta;
	result.m[2] = -sin_theta;
	result.m[10] = cos_theta;
	return result;
}

// Returns matrix m rotated about z-axis by deg degrees
inline mat4 rotate_z_deg(mat4 m, float deg) {
	float rad = DEG2RAD(deg);
	mat4 m_r = identity_mat4();
	float sin_theta = sinf(rad);
	float cos_theta = cosf(rad);
	m_r.m[0] = cos_theta;
	m_r.m[4] = -sin_theta;
	m_r.m[1] = sin_theta;
	m_r.m[5] = cos_theta;
	return m_r * m;
}

// Returns matrix to rotate about z-axis by deg degrees
inline mat4 rotate_z_deg_mat4(float deg) {
	float rad = DEG2RAD(deg);
	mat4 result = identity_mat4();
	float sin_theta = sinf(rad);
	float cos_theta = cosf(rad);
	result.m[0] = cos_theta;
	result.m[4] = -sin_theta;
	result.m[1] = sin_theta;
	result.m[5] = cos_theta;
	return result;
}

// Returns rotation matrix to rotate around (NORMALISED!) axis 'u' by 'deg' degrees
//from http://www.iquilezles.org/www/articles/noacos/noacos.htm
inline mat4 rotate_axis_deg_mat4(vec3 u, float deg){
	float rad = DEG2RAD(deg);

	float sin_a = sinf(rad);
	float cos_a = cosf(rad);
	float inv_cos_a = 1.0f - cos_a;

	return mat4 {
		u.x*u.x*inv_cos_a + cos_a,
		u.x*u.y*inv_cos_a + sin_a*u.z,
		u.x*u.z*inv_cos_a - sin_a*u.y,
		0,
		u.y*u.x*inv_cos_a - sin_a*u.z,
		u.y*u.y*inv_cos_a + cos_a,
		u.y*u.z*inv_cos_a + sin_a*u.x,
		0,
		u.z*u.x*inv_cos_a + sin_a*u.y,
		u.z*u.y*inv_cos_a - sin_a*u.x,
		u.z*u.z*inv_cos_a + cos_a,
		0,
		0, 0, 0, 1
	};
}

// Returns rotation matrix to align u1 with u2 (MUST BE UNIT VECTORS)
//from http://www.iquilezles.org/www/articles/noacos/noacos.htm
inline mat4 rotate_align_mat4(vec3 u1, vec3 u2){
	vec3 axis = cross(u1,u2);
	float cos_a = dot(u1,u2);
    float k = 1.0f/(1.0f+cos_a);

	if(almost_equal(cos_a,-1)) //vectors are opposite
		return scale(identity_mat4(),-1);

	return mat4{
		axis.x*axis.x*k + cos_a,
		axis.x*axis.y*k + axis.z,
		axis.x*axis.z*k - axis.y,
		0,
		axis.y*axis.x*k - axis.z,
		axis.y*axis.y*k + cos_a,
		axis.y*axis.z*k + axis.x,
		0,
		axis.z*axis.x*k + axis.y,
		axis.z*axis.y*k - axis.x,
		axis.z*axis.z*k + cos_a,
		0,
		0, 0, 0, 1
	};
}

// Returns matrix m scaled by [x, y, z]
inline mat4 scale(mat4 m, vec3 v) {
	mat4 result = m;
	result.m[0] *= v.x;
	result.m[1] *= v.x;
	result.m[2] *= v.x;
	result.m[3] *= v.x;

	result.m[4] *= v.y;
	result.m[5] *= v.y;
	result.m[6] *= v.y;
	result.m[7] *= v.y;

	result.m[8] *= v.z;
	result.m[9] *= v.z;
	result.m[10] *= v.z;
	result.m[11] *= v.z;
	return result;
}

// Returns matrix to scale by [x, y, z]
inline mat4 scale_mat4(vec3 v) {
	mat4 result = {};
	result.m[0] = v.x;
	result.m[5] = v.y;
	result.m[10] = v.z;
	result.m[15] = 1.0f;
	return result;
}

// Returns matrix m scaled uniformly by s
inline mat4 scale(mat4 m, float s) {
	mat4 result = m;
	result.m[0] *= s;
	result.m[1] *= s;
	result.m[2] *= s;
	result.m[3] *= s;
	result.m[4] *= s;
	result.m[5] *= s;
	result.m[6] *= s;
	result.m[7] *= s;
	result.m[8] *= s;
	result.m[9] *= s;
	result.m[10] *= s;
	result.m[11] *= s;
	return result;
}

/*-----------------------VIRTUAL CAMERA MATRIX FUNCTIONS----------------------*/
// returns a view matrix using the opengl lookAt style. COLUMN ORDER.
inline mat4 look_at(vec3 cam_pos, vec3 target_pos, vec3 up) {
	vec3 fwd = normalise(target_pos - cam_pos);
	vec3 rgt = normalise(cross(fwd, up));
	vec3 up_act = normalise(cross(rgt, fwd));

	mat4 result = identity_mat4();
	result.m[0] = rgt.x;
	result.m[4] = rgt.y;
	result.m[8] = rgt.z;
	result.m[1] = up_act.x;
	result.m[5] = up_act.y;
	result.m[9] = up_act.z;
	result.m[2] = -fwd.x;
	result.m[6] = -fwd.y;
	result.m[10] = -fwd.z;
	result.m[12] = -cam_pos.x;
	result.m[13] = -cam_pos.y;
	result.m[14] = -cam_pos.z;
	/* result = {
		Rx  Ry  Rz -Px
		Uy  Uy  Uz -Py
	   -Fx -Fy -Fz -Pz
		0   0   0   1
	} */
	return result;
}

// Returns an orthographic projection matrix
inline mat4 orthographic(float left, float right, float bottom, float top, float near_z, float far_z) {
	mat4 result = identity_mat4(); 
	result.m[0] = 2/(right-left);
	result.m[5] = 2/(top-bottom);
	result.m[10] = 2/(far_z-near_z);
	result.m[12] = -(right+left)/(right-left);
	result.m[13] = -(top+bottom)/(top-bottom);
	result.m[14] = (far_z+near_z)/(far_z-near_z);
	return result;
}

// Returns a perspective projection matrix
inline mat4 perspective(float fov, float aspect_ratio, float near_z, float far_z) {
	float fov_rad = DEG2RAD(fov/aspect_ratio);
	float range = tanf(fov_rad / 2.0f) * near_z;
	float sx = (2.0f * near_z) / (range * aspect_ratio + range * aspect_ratio);
	float sy = near_z / range;
	float sz = -(far_z + near_z) / (far_z - near_z);
	float pz = -(2.0f * far_z * near_z) / (far_z - near_z);
	mat4 result = {}; // make sure bottom-right corner is zero
	result.m[0] = sx;
	result.m[5] = sy;
	result.m[10] = sz;
	result.m[11] = -1.0f;
	result.m[14] = pz;
	return result;
}

/*----------------------------HAMILTON IN DA HOUSE!---------------------------*/
inline versor quat_from_axis_rad(float radians, float x, float y, float z) {
	versor result;
	float sin_half_theta = sinf(0.5f*radians);
	result.q[0] = cosf(0.5f*radians);
	result.q[1] = sin_half_theta * x;
	result.q[2] = sin_half_theta * y;
	result.q[3] = sin_half_theta * z;
	return result;
}

inline versor quat_from_axis_deg(float degrees, float x, float y, float z) {
	return quat_from_axis_rad(DEG2RAD(degrees), x, y, z);
}

inline versor quat_from_axis_deg(float degrees, vec3 xyz) {
	return quat_from_axis_rad(DEG2RAD(degrees), xyz.x, xyz.y, xyz.z);
}

inline mat4 quat_to_mat4(versor q) {
	float w = q.q[0];
	float x = q.q[1];
	float y = q.q[2];
	float z = q.q[3];
	return mat4 {
		1.0f - (2.0f * y * y) - (2.0f * z * z),
		(2.0f * x * y) + (2.0f * w * z),
		(2.0f * x * z) - (2.0f * w * y),
		0.0f,
		(2.0f * x * y) - (2.0f * w * z),
		1.0f - (2.0f * x * x) - (2.0f * z * z),
		(2.0f * y * z) + (2.0f * w * x),
		0.0f,
		(2.0f * x * z) + (2.0f * w * y),
		(2.0f * y * z) - (2.0f * w * x),
		1.0f - (2.0f * x * x) - (2.0f * y * y),
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		1.0f
	};
}

inline versor normalise(versor q) {
	versor result = q;
	float mag_sq = (q.q[0] * q.q[0]) + (q.q[1] * q.q[1]) +
				   (q.q[2] * q.q[2]) + (q.q[3] * q.q[3]);
	
	const float thresh = 0.0001f;
	if(almost_equal(mag_sq, 1.0f, thresh)) {
		result = result / sqrtf(mag_sq);
	}
	return result;
}

inline float dot(versor q, versor r) {
	return ((q.q[0] * r.q[0]) + (q.q[1] * r.q[1]) + (q.q[2] * r.q[2]) + (q.q[3] * r.q[3]));
}

inline versor slerp(versor& q, versor& r, float t) {
	// angle between q0-q1
	float cos_half_theta = dot(q, r);
	// as found here http://stackoverflow.com/questions/2886606/flipping-issue-when-interpolating-rotations-using-quaternions
	// if dot product is negative then one quaternion should be negated, to make
	// it take the short way around, rather than the long way
	// yeah! and furthermore Susan, I had to recalculate the d.p. after this
	if(cos_half_theta < 0.0f) {
		q = -q;
		cos_half_theta = dot(q, r);
	}
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if(fabs(cos_half_theta) >= 1.0f) {
		return q;
	}
	// Calculate temporary values
	float sin_half_theta = sqrtf(1.0f - cos_half_theta * cos_half_theta);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	versor result;
	if(fabs(sin_half_theta) < 0.001f) {
		for(int i = 0; i < 4; ++i) {
			result.q[i] = (1.0f - t) * q.q[i] + t * r.q[i];
		}
		return result;
	}
	float half_theta = acosf(cos_half_theta);
	float a = sinf((1.0f - t) * half_theta) / sin_half_theta;
	float b = sinf(t * half_theta) / sin_half_theta;
	for(int i = 0; i < 4; ++i) {
		result.q[i] = q.q[i] * a + r.q[i] * b;
	}
	return result;
}

/*-----------------------------PRINT FUNCTIONS--------------------------------*/
inline void print(vec2 v) {
	printf("[%.2f, %.2f]\n", v.x, v.y);
}

inline void print(vec3 v) {
	printf("[%.2f, %.2f, %.2f]\n", v.x, v.y, v.z);
}

inline void print(vec4 v) {
	printf("[%.2f, %.2f, %.2f, %.2f]\n", v.x, v.y, v.z, v.w);
}

inline void print(mat3 m) {
	printf("\n");
	printf("[%.2f][%.2f][%.2f]\n", m.m[0], m.m[3], m.m[6]);
	printf("[%.2f][%.2f][%.2f]\n", m.m[1], m.m[4], m.m[7]);
	printf("[%.2f][%.2f][%.2f]\n", m.m[2], m.m[5], m.m[8]);
}

inline void print(mat4 m) {
	printf("\n");
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[0], m.m[4], m.m[8], m.m[12]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[1], m.m[5], m.m[9], m.m[13]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[2], m.m[6], m.m[10], m.m[14]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[3], m.m[7], m.m[11], m.m[15]);
}

inline void print(versor q) {
	printf("[%.2f ,%.2f, %.2f, %.2f]\n", q.q[0], q.q[1], q.q[2], q.q[3]);
}