#pragma once
//Kevin's game maths library
//Based off simple maths library included with online source for 'Anton's OpenGL Tutorials' book by Anton Gerdelan
//License: https://github.com/capnramses/antons_opengl_tutorials_book/blob/master/LICENCE.md
//Modified and added to by me over the years
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

#define _USE_MATH_DEFINES
#include <math.h>

#define MIN(a,b) (((a)<(b)) ? (a) : (b))
#define MAX(a,b) (((a)>(b)) ? (a) : (b))
#define CLAMP(x,lo,hi) (MIN ((hi), MAX ((lo), (x))))

//Compare two floats for equality
bool cmpf(float a, float b, float eps = 0.000001f);

float DEG2RAD(float degs);
float RAD2DEG(float rads);

// data structures
struct vec2;
struct vec3;
struct vec4;
struct mat3;
struct mat4;
struct versor;

// vector functions
float length(const vec2& v);
float length2(const vec2& v);
vec2 normalise(const vec2& v);
float dot(const vec2& a, const vec2& b);
float get_squared_dist(vec2 from, vec2 to);

float length(const vec3& v);
float length2(const vec3& v);
vec3 normalise(const vec3& v);
float dot(const vec3& a, const vec3& b);
vec3 cross(const vec3& a, const vec3& b);
float get_squared_dist(vec3 from, vec3 to);
float direction_to_heading(vec2 d);
vec2 heading_to_direction(float degrees);

// matrix functions
mat4 zero_mat4();
mat4 identity_mat4();
float determinant(const mat4& mm);
mat4 inverse(const mat4& mm);
mat4 transpose(const mat4& mm);

// affine functions
mat4 translate(const mat4& m, const vec3& v);
mat4 rotate_x_deg(const mat4& m, float deg);
mat4 rotate_y_deg(const mat4& m, float deg);
mat4 rotate_z_deg(const mat4& m, float deg);
mat4 rotate_axis_deg(const vec3& u, float a);
mat4 rotate_align(const vec3& u1, const vec3& u2);
mat4 scale(const mat4& m, const vec3& v);
mat4 scale(const mat4& m, float s);

// camera functions
mat4 look_at(const vec3& cam_pos, vec3 targ_pos, const vec3& up);
mat4 orthographic(float left, float right, float bottom, float top, float near_z, float far_z);
mat4 perspective(float fovy, float aspect, float near_z, float far_z);

// quaternion functions
versor quat_from_axis_rad(float radians, float x, float y, float z);
versor quat_from_axis_deg(float degrees, float x, float y, float z);
versor quat_from_axis_deg(float degrees, vec3 a);
mat4 quat_to_mat4(const versor& q);
float dot(const versor& q, const versor& r);
versor slerp(const versor& q, const versor& r);
// stupid overloading wouldn't let me use const
versor normalise(versor& q);
float dot(const versor& q, const versor& r);
versor slerp(versor& q, versor& r, float t);

// print functions
void print(const vec2& v);
void print(const vec3& v);
void print(const vec4& v);
void print(const mat3& m);
void print(const mat4& m);
void print(const versor& q);

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

struct vec2 {
	union {
		struct{
			float v[2];
		};
		struct{
			float x, y;
		};
	};

	vec2();
	vec2(float x, float y);
	vec2(vec3 v3); //fwd dec, can't define before vec3

	vec2 operator+ (const vec2& rhs);
	vec2& operator+= (const vec2& rhs);
	vec2 operator- (const vec2& rhs);
	vec2& operator-= (const vec2& rhs);
	vec2 operator+ (float rhs);
	vec2 operator- (float rhs);
	vec2 operator* (float rhs);
	vec2 operator/ (float rhs);
	vec2& operator*= (float rhs);
	vec2& operator= (const vec2& rhs);
	vec2 operator- ();
	bool operator== (const vec2& rhs);
};

struct vec3 {
	union {
		struct{
			float v[3];
		};
		struct {
			float x, y, z;
		};
	};

	vec3();
	vec3(float x, float y, float z);
	vec3(const vec2& vv, float z);
	vec3(const vec4& vv);

	vec3 operator+ (const vec3& rhs);
	vec3& operator+= (const vec3& rhs);
	vec3 operator- (const vec3& rhs);
	vec3& operator-= (const vec3& rhs);
	vec3 operator+ (float rhs);
	vec3 operator- (float rhs);
	vec3 operator* (float rhs);
	vec3 operator/ (float rhs);
	vec3& operator*= (float rhs);
	vec3& operator= (const vec3& rhs);
	vec3 operator- ();
	bool operator== (const vec3& rhs);
};

struct vec4 {
	union{
		struct{
			float v[4];
		};
		struct{
			float x, y, z, w;
		};
		struct{
			float r, g, b, a;
		};
	};

	vec4();
	vec4(float x, float y, float z, float w);
	vec4(const vec2& vv, float z, float w);
	vec4(const vec3& vv, float w);
};

struct mat3 {
	float m[9];
	/* note: entered in COLUMNS. Stored like this:
		0 3 6
		1 4 7
		2 5 8 
	*/
	mat3();
	mat3(float a, float b, float c, float d, float e, float f, float g, float h, float i);
	mat3(mat4);

	vec3 operator* (const vec3& rhs);
};

struct mat4 {
	float m[16];
	/* stored like this:
		0  4  8 12
		1  5  9 13
		2  6 10 14
		3  7 11 15
	*/

	mat4();
	mat4(float a, float b, float c, float d, float e, float f, float g, float h,
		  float i, float j, float k, float l, float mm, float n, float o, float p);
	vec4 operator* (const vec4& rhs);
	mat4 operator* (const mat4& rhs);
	mat4& operator= (const mat4& rhs);
};

struct versor {
	float q[4];

	versor();
	versor operator/ (float rhs);
	versor operator* (float rhs);
	versor operator* (const versor& rhs);
	versor operator+ (const versor& rhs);
};

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef __clang__
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif
