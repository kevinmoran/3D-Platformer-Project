#include "GameMaths.h"

#include <stdio.h>

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

//Compare two floats for equality
bool cmpf(float a, float b, float eps) {
	return (fabs(a-b) < eps);
}

float DEG2RAD(float degs) {
	return degs*(M_PI/180.0);
}
float RAD2DEG(float rads) {
	return rads*(180.0/M_PI);
}

//------------------------------------------------------------------------------
//vec2
//------------------------------------------------------------------------------
vec2::vec2() {}

vec2::vec2(float x, float y) {
	v[0] = x;
	v[1] = y;
}

vec2::vec2(vec3 v3) {//Truncation ctor
	v[0] = v3.v[0];
	v[1] = v3.v[1];
}

vec2 vec2::operator+ (const vec2& rhs) {
	vec2 vc;
	vc.v[0] = v[0] + rhs.v[0];
	vc.v[1] = v[1] + rhs.v[1];
	return vc;
}

vec2& vec2::operator+= (const vec2& rhs) {
	v[0] += rhs.v[0];
	v[1] += rhs.v[1];
	return *this; // return self
}

vec2 vec2::operator- (const vec2& rhs) {
	vec2 vc;
	vc.v[0] = v[0] - rhs.v[0];
	vc.v[1] = v[1] - rhs.v[1];
	return vc;
}

vec2& vec2::operator-= (const vec2& rhs) {
	v[0] -= rhs.v[0];
	v[1] -= rhs.v[1];
	return *this;
}

vec2 vec2::operator+ (float rhs) {
	vec2 vc;
	vc.v[0] = v[0] + rhs;
	vc.v[1] = v[1] + rhs;
	return vc;
}

vec2 vec2::operator- (float rhs) {
	vec2 vc;
	vc.v[0] = v[0] - rhs;
	vc.v[1] = v[1] - rhs;
	return vc;
}

vec2 vec2::operator* (float rhs) {
	vec2 vc;
	vc.v[0] = v[0] * rhs;
	vc.v[1] = v[1] * rhs;
	return vc;
}

vec2 vec2::operator/ (float rhs) {
	vec2 vc;
	vc.v[0] = v[0] / rhs;
	vc.v[1] = v[1] / rhs;
	return vc;
}

vec2& vec2::operator*= (float rhs) {
	v[0] = v[0] * rhs;
	v[1] = v[1] * rhs;
	return *this;
}

vec2& vec2::operator= (const vec2& rhs) {
	v[0] = rhs.v[0];
	v[1] = rhs.v[1];
	return *this;
}
vec2 vec2::operator- () {
	vec2 vc;
	vc.v[0] = v[0] * (-1);
	vc.v[1] = v[1] * (-1);
	return vc;
}
bool vec2::operator== (const vec2& rhs) {
	return (cmpf(v[0], rhs.v[0]) &&
			cmpf(v[1], rhs.v[1]));
}

//------------------------------------------------------------------------------
//vec3
//------------------------------------------------------------------------------
vec3::vec3() {}
vec3::vec3 (float x, float y, float z) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
}
vec3::vec3(const vec2& vv, float z) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = z;
}
vec3::vec3(const vec4& vv) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = vv.v[2];
}

vec3 vec3::operator+ (const vec3& rhs) {
	vec3 vc;
	vc.v[0] = v[0] + rhs.v[0];
	vc.v[1] = v[1] + rhs.v[1];
	vc.v[2] = v[2] + rhs.v[2];
	return vc;
}
vec3& vec3::operator+= (const vec3& rhs) {
	v[0] += rhs.v[0];
	v[1] += rhs.v[1];
	v[2] += rhs.v[2];
	return *this; // return self
}
vec3 vec3::operator- (const vec3& rhs) {
	vec3 vc;
	vc.v[0] = v[0] - rhs.v[0];
	vc.v[1] = v[1] - rhs.v[1];
	vc.v[2] = v[2] - rhs.v[2];
	return vc;
}
vec3& vec3::operator-= (const vec3& rhs) {
	v[0] -= rhs.v[0];
	v[1] -= rhs.v[1];
	v[2] -= rhs.v[2];
	return *this;
}
vec3 vec3::operator+ (float rhs) {
	vec3 vc;
	vc.v[0] = v[0] + rhs;
	vc.v[1] = v[1] + rhs;
	vc.v[2] = v[2] + rhs;
	return vc;
}
vec3 vec3::operator- (float rhs) {
	vec3 vc;
	vc.v[0] = v[0] - rhs;
	vc.v[1] = v[1] - rhs;
	vc.v[2] = v[2] - rhs;
	return vc;
}
vec3 vec3::operator* (float rhs) {
	vec3 vc;
	vc.v[0] = v[0] * rhs;
	vc.v[1] = v[1] * rhs;
	vc.v[2] = v[2] * rhs;
	return vc;
}
vec3 vec3::operator/ (float rhs) {
	vec3 vc;
	vc.v[0] = v[0] / rhs;
	vc.v[1] = v[1] / rhs;
	vc.v[2] = v[2] / rhs;
	return vc;
}
vec3& vec3::operator*= (float rhs) {
	v[0] = v[0] * rhs;
	v[1] = v[1] * rhs;
	v[2] = v[2] * rhs;
	return *this;
}
vec3& vec3::operator= (const vec3& rhs) {
	v[0] = rhs.v[0];
	v[1] = rhs.v[1];
	v[2] = rhs.v[2];
	return *this;
}
vec3 vec3::operator- () {
	vec3 vc;
	vc.v[0] = v[0] * (-1);
	vc.v[1] = v[1] * (-1);
	vc.v[2] = v[2] * (-1);
	return vc;
}
bool vec3::operator== (const vec3& rhs) {
	return (cmpf(v[0], rhs.v[0]) && 
			cmpf(v[1], rhs.v[1]) &&
			cmpf(v[2], rhs.v[2]));
}

//------------------------------------------------------------------------------
//vec4
//------------------------------------------------------------------------------
vec4::vec4() {}
vec4::vec4(float x, float y, float z, float w) {
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}
vec4::vec4(const vec2& vv, float z, float w) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = z;
	v[3] = w;
}
vec4::vec4(const vec3& vv, float w) {
	v[0] = vv.v[0];
	v[1] = vv.v[1];
	v[2] = vv.v[2];
	v[3] = w;
}

//------------------------------------------------------------------------------
//mat3
//------------------------------------------------------------------------------
mat3::mat3() {}
mat3::mat3(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
	m[0] = a; m[1] = b; m[2] = c;
	m[3] = d; m[4] = e; m[5] = f;
	m[6] = g; m[7] = h; m[8] = i;
}

mat3::mat3(mat4 RTS){
	//Grab R and S from RTS matrix
	m[0] = RTS.m[0];
	m[1] = RTS.m[1];
	m[2] = RTS.m[2];
	m[3] = RTS.m[4];
	m[4] = RTS.m[5];
	m[5] = RTS.m[6];
	m[6] = RTS.m[8];
	m[7] = RTS.m[9];
	m[8] = RTS.m[10];
}

vec3 mat3::operator* (const vec3& rhs) {
	// 0x + 3y + 6z
	float x =
		m[0] * rhs.v[0] +
		m[3] * rhs.v[1] +
		m[6] * rhs.v[2];
	// 1x + 4y + 7z
	float y = 
		m[1] * rhs.v[0] +
		m[4] * rhs.v[1] +
		m[7] * rhs.v[2];
	// 2x + 5y + 8z
	float z = 
		m[2] * rhs.v[0] +
		m[5] * rhs.v[1] +
		m[8] * rhs.v[2];
	return vec3 (x, y, z);
}

//------------------------------------------------------------------------------
//mat4
//------------------------------------------------------------------------------
mat4::mat4() {}
mat4::mat4(float a, float b, float c, float d, float e, float f, float g, float h,
		float i, float j, float k, float l, float mm, float n, float o, float p) {
	m[0] = a; m[1] = b; m[2] = c; m[3] = d;
	m[4] = e; m[5] = f; m[6] = g; m[7] = h;
	m[8] = i; m[9] = j; m[10] = k; m[11] = l;
	m[12] = mm; m[13] = n; m[14] = o; m[15] = p;
}
vec4 mat4::operator* (const vec4& rhs) {
	// 0x + 4y + 8z + 12w
	float x =
		m[0] * rhs.v[0] +
		m[4] * rhs.v[1] +
		m[8] * rhs.v[2] +
		m[12] * rhs.v[3];
	// 1x + 5y + 9z + 13w
	float y = 
		m[1] * rhs.v[0] +
		m[5] * rhs.v[1] +
		m[9] * rhs.v[2] +
		m[13] * rhs.v[3];
	// 2x + 6y + 10z + 14w
	float z = 
		m[2] * rhs.v[0] +
		m[6] * rhs.v[1] +
		m[10] * rhs.v[2] +
		m[14] * rhs.v[3];
	// 3x + 7y + 11z + 15w
	float w = 
		m[3] * rhs.v[0] +
		m[7] * rhs.v[1] +
		m[11] * rhs.v[2] +
		m[15] * rhs.v[3];
	return vec4 (x, y, z, w);
}
mat4 mat4::operator* (const mat4& rhs) {
	mat4 r = zero_mat4 ();
	int r_index = 0;
	for(int col = 0; col < 4; col++) {
		for(int row = 0; row < 4; row++) {
			float sum = 0.0f;
			for(int i = 0; i < 4; i++) {
				sum += rhs.m[i + col * 4] * m[row + i * 4];
			}
			r.m[r_index] = sum;
			r_index++;
		}
	}
	return r;
}
mat4& mat4::operator= (const mat4& rhs) {
	for(int i = 0; i < 16; i++) {
		m[i] = rhs.m[i];
	}
	return *this;
}

//versor
versor::versor() {}
versor versor::operator/ (float rhs) {
	versor result;
	result.q[0] = q[0] / rhs;
	result.q[1] = q[1] / rhs;
	result.q[2] = q[2] / rhs;
	result.q[3] = q[3] / rhs;
	return result;
}
versor versor::operator* (float rhs) {
	versor result;
	result.q[0] = q[0] * rhs;
	result.q[1] = q[1] * rhs;
	result.q[2] = q[2] * rhs;
	result.q[3] = q[3] * rhs;
	return result;
}
versor versor::operator* (const versor& rhs) {
	versor result;
	result.q[0] = rhs.q[0] * q[0] - rhs.q[1] * q[1] -
		rhs.q[2] * q[2] - rhs.q[3] * q[3];
	result.q[1] = rhs.q[0] * q[1] + rhs.q[1] * q[0] -
		rhs.q[2] * q[3] + rhs.q[3] * q[2];
	result.q[2] = rhs.q[0] * q[2] + rhs.q[1] * q[3] +
		rhs.q[2] * q[0] - rhs.q[3] * q[1];
	result.q[3] = rhs.q[0] * q[3] - rhs.q[1] * q[2] +
		rhs.q[2] * q[1] + rhs.q[3] * q[0];
	// re-normalise in case of mangling
	return normalise(result);
}
versor versor::operator+ (const versor& rhs) {
	versor result;
	result.q[0] = rhs.q[0] + q[0];
	result.q[1] = rhs.q[1] + q[1];
	result.q[2] = rhs.q[2] + q[2];
	result.q[3] = rhs.q[3] + q[3];
	// re-normalise in case of mangling
	return normalise(result);
}

/*------------------------------VECTOR FUNCTIONS------------------------------*/
//---vec2---//
inline float length(const vec2& v) {
	return sqrt(v.v[0] * v.v[0] + v.v[1] * v.v[1]);
}
// squared length
inline float length2(const vec2& v) {
	return v.v[0] * v.v[0] + v.v[1] * v.v[1];
}

// returns unit vector in direction of v
inline vec2 normalise(const vec2& v) {
	vec2 vb;
	float l = length(v);
	if(0.0f == l) {
		return vec2(0.0f, 0.0f);
	}
	vb.v[0] = v.v[0] / l;
	vb.v[1] = v.v[1] / l;
	return vb;
}
inline float dot(const vec2& a, const vec2& b) {
	return a.v[0] * b.v[0] + a.v[1] * b.v[1];
}
inline float get_squared_dist(vec2 from, vec2 to) {
	float x = (to.v[0] - from.v[0]) * (to.v[0] - from.v[0]);
	float y = (to.v[1] - from.v[1]) * (to.v[1] - from.v[1]);
	return x + y;
}

//---vec3---//
inline float length(const vec3& v) {
	return sqrt (v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2]);
}
// squared length
inline float length2(const vec3& v) {
	return v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
}
// squared length on xz plane
inline float length2_xz(const vec3& v) {
	return v.v[0] * v.v[0] + v.v[2] * v.v[2];
}
// returns unit vector in direction of v
inline vec3 normalise(const vec3& v) {
	vec3 vb;
	float l = length(v);
	if(0.0f == l) {
		return vec3(0.0f, 0.0f, 0.0f);
	}
	vb.v[0] = v.v[0] / l;
	vb.v[1] = v.v[1] / l;
	vb.v[2] = v.v[2] / l;
	return vb;
}
inline float dot(const vec3& a, const vec3& b) {
	return a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2];
}
inline vec3 cross(const vec3& a, const vec3& b) {
	float x = a.v[1] * b.v[2] - a.v[2] * b.v[1];
	float y = a.v[2] * b.v[0] - a.v[0] * b.v[2];
	float z = a.v[0] * b.v[1] - a.v[1] * b.v[0];
	return vec3(x, y, z);
}
inline float get_squared_dist(vec3 from, vec3 to) {
	float x = (to.v[0] - from.v[0]) * (to.v[0] - from.v[0]);
	float y = (to.v[1] - from.v[1]) * (to.v[1] - from.v[1]);
	float z = (to.v[2] - from.v[2]) * (to.v[2] - from.v[2]);
	return x + y + z;
}
//converts a 2D direction vector into a heading in degrees
//angle vector makes with positive x-axis
inline float direction_to_heading(vec2 dir) {
	return RAD2DEG(atan2(dir.v[1], dir.v[0]));
}
//converts an angle in degrees to a 2D vector which makes that angle with the positive x-axis
inline vec2 heading_to_direction(float degrees) {
	float rad = DEG2RAD(degrees);
	return vec2(cos(rad), sin(rad));
	//return vec3 (-sinf (rad), 0.0f, -cosf (rad));
}

/*-----------------------------MATRIX FUNCTIONS-------------------------------*/
/*
inline mat3 zero_mat3() {
	return mat3 (
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f
	);
}
*/
/*
inline mat3 identity_mat3() {
	return mat3 (
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}
*/
inline mat4 zero_mat4() {
	return mat4 (
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	);
}

inline mat4 identity_mat4() {
	return mat4 (
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	/* mat4 array layout
	 0  4  8 12
	 1  5  9 13
	 2  6 10 14
	 3  7 11 15
	*/
}

// returns a scalar value with the determinant for a 4x4 matrix
// see http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/fourD/index.htm
inline float determinant(const mat4& mm) {
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
inline mat4 inverse(const mat4& mm) {
	float det = determinant(mm);
	/* there is no inverse if determinant is zero (not likely unless scale is
	broken) */
	if(0.0f == det) {
		fprintf(stderr, "WARNING. matrix has no determinant. can not invert\n");
		return mm;
	}
	float inv_det = 1.0f / det;
	
	return mat4(
		inv_det * (
			mm.m[9] * mm.m[14] * mm.m[7] - mm.m[13] * mm.m[10] * mm.m[7] +
			mm.m[13] * mm.m[6] * mm.m[11] - mm.m[5] * mm.m[14] * mm.m[11] -
			mm.m[9] * mm.m[6] * mm.m[15] + mm.m[5] * mm.m[10] * mm.m[15]
		),
		inv_det * (
			mm.m[13] * mm.m[10] * mm.m[3] - mm.m[9] * mm.m[14] * mm.m[3] -
			mm.m[13] * mm.m[2] * mm.m[11] + mm.m[1] * mm.m[14] * mm.m[11] +
			mm.m[9] * mm.m[2] * mm.m[15] - mm.m[1] * mm.m[10] * mm.m[15]
		),
		inv_det * (
			mm.m[5] * mm.m[14] * mm.m[3] - mm.m[13] * mm.m[6] * mm.m[3] +
			mm.m[13] * mm.m[2] * mm.m[7] - mm.m[1] * mm.m[14] * mm.m[7] -
			mm.m[5] * mm.m[2] * mm.m[15] + mm.m[1] * mm.m[6] * mm.m[15]
		),
		inv_det * (
			mm.m[9] * mm.m[6] * mm.m[3] - mm.m[5] * mm.m[10] * mm.m[3] -
			mm.m[9] * mm.m[2] * mm.m[7] + mm.m[1] * mm.m[10] * mm.m[7] +
			mm.m[5] * mm.m[2] * mm.m[11] - mm.m[1] * mm.m[6] * mm.m[11]
		),
		inv_det * (
			mm.m[12] * mm.m[10] * mm.m[7] - mm.m[8] * mm.m[14] * mm.m[7] -
			mm.m[12] * mm.m[6] * mm.m[11] + mm.m[4] * mm.m[14] * mm.m[11] +
			mm.m[8] * mm.m[6] * mm.m[15] - mm.m[4] * mm.m[10] * mm.m[15]
		),
		inv_det * (
			mm.m[8] * mm.m[14] * mm.m[3] - mm.m[12] * mm.m[10] * mm.m[3] +
			mm.m[12] * mm.m[2] * mm.m[11] - mm.m[0] * mm.m[14] * mm.m[11] -
			mm.m[8] * mm.m[2] * mm.m[15] + mm.m[0] * mm.m[10] * mm.m[15]
		),
		inv_det * (
			mm.m[12] * mm.m[6] * mm.m[3] - mm.m[4] * mm.m[14] * mm.m[3] -
			mm.m[12] * mm.m[2] * mm.m[7] + mm.m[0] * mm.m[14] * mm.m[7] +
			mm.m[4] * mm.m[2] * mm.m[15] - mm.m[0] * mm.m[6] * mm.m[15]
		),
		inv_det * (
			mm.m[4] * mm.m[10] * mm.m[3] - mm.m[8] * mm.m[6] * mm.m[3] +
			mm.m[8] * mm.m[2] * mm.m[7] - mm.m[0] * mm.m[10] * mm.m[7] -
			mm.m[4] * mm.m[2] * mm.m[11] + mm.m[0] * mm.m[6] * mm.m[11]
		),
		inv_det * (
			mm.m[8] * mm.m[13] * mm.m[7] - mm.m[12] * mm.m[9] * mm.m[7] +
			mm.m[12] * mm.m[5] * mm.m[11] - mm.m[4] * mm.m[13] * mm.m[11] -
			mm.m[8] * mm.m[5] * mm.m[15] + mm.m[4] * mm.m[9] * mm.m[15]
		),
		inv_det * (
			mm.m[12] * mm.m[9] * mm.m[3] - mm.m[8] * mm.m[13] * mm.m[3] -
			mm.m[12] * mm.m[1] * mm.m[11] + mm.m[0] * mm.m[13] * mm.m[11] +
			mm.m[8] * mm.m[1] * mm.m[15] - mm.m[0] * mm.m[9] * mm.m[15]
		),
		inv_det * (
			mm.m[4] * mm.m[13] * mm.m[3] - mm.m[12] * mm.m[5] * mm.m[3] +
			mm.m[12] * mm.m[1] * mm.m[7] - mm.m[0] * mm.m[13] * mm.m[7] -
			mm.m[4] * mm.m[1] * mm.m[15] + mm.m[0] * mm.m[5] * mm.m[15]
		),
		inv_det * (
			mm.m[8] * mm.m[5] * mm.m[3] - mm.m[4] * mm.m[9] * mm.m[3] -
			mm.m[8] * mm.m[1] * mm.m[7] + mm.m[0] * mm.m[9] * mm.m[7] +
			mm.m[4] * mm.m[1] * mm.m[11] - mm.m[0] * mm.m[5] * mm.m[11]
		),
		inv_det * (
			mm.m[12] * mm.m[9] * mm.m[6] - mm.m[8] * mm.m[13] * mm.m[6] -
			mm.m[12] * mm.m[5] * mm.m[10] + mm.m[4] * mm.m[13] * mm.m[10] +
			mm.m[8] * mm.m[5] * mm.m[14] - mm.m[4] * mm.m[9] * mm.m[14]
		),
		inv_det * (
			mm.m[8] * mm.m[13] * mm.m[2] - mm.m[12] * mm.m[9] * mm.m[2] +
			mm.m[12] * mm.m[1] * mm.m[10] - mm.m[0] * mm.m[13] * mm.m[10] -
			mm.m[8] * mm.m[1] * mm.m[14] + mm.m[0] * mm.m[9] * mm.m[14]
		),
		inv_det * (
			mm.m[12] * mm.m[5] * mm.m[2] - mm.m[4] * mm.m[13] * mm.m[2] -
			mm.m[12] * mm.m[1] * mm.m[6] + mm.m[0] * mm.m[13] * mm.m[6] +
			mm.m[4] * mm.m[1] * mm.m[14] - mm.m[0] * mm.m[5] * mm.m[14]
		),
		inv_det * (
			mm.m[4] * mm.m[9] * mm.m[2] - mm.m[8] * mm.m[5] * mm.m[2] +
			mm.m[8] * mm.m[1] * mm.m[6] - mm.m[0] * mm.m[9] * mm.m[6] -
			mm.m[4] * mm.m[1] * mm.m[10] + mm.m[0] * mm.m[5] * mm.m[10]
		)
	);
}

// returns a 16-element array flipped on the main diagonal
inline mat4 transpose(const mat4& mm) {
	return mat4(
		mm.m[0], mm.m[4], mm.m[8], mm.m[12],
		mm.m[1], mm.m[5], mm.m[9], mm.m[13],
		mm.m[2], mm.m[6], mm.m[10], mm.m[14],
		mm.m[3], mm.m[7], mm.m[11], mm.m[15]
	);
}

/*--------------------------AFFINE MATRIX FUNCTIONS---------------------------*/
// translate a 4d matrix with xyz array
inline mat4 translate(const mat4& m, const vec3& v) {
	mat4 m_t = identity_mat4();
	m_t.m[12] = v.v[0];
	m_t.m[13] = v.v[1];
	m_t.m[14] = v.v[2];
	return m_t * m;
}

// rotate around x axis by an angle in degrees
inline mat4 rotate_x_deg(const mat4& m, float deg) {
	// convert to radians
	float rad = DEG2RAD(deg);
	mat4 m_r = identity_mat4();
	m_r.m[5] = cos(rad);
	m_r.m[9] = -sin(rad);
	m_r.m[6] = sin(rad);
	m_r.m[10] = cos(rad);
	return m_r * m;
}

// rotate around y axis by an angle in degrees
inline mat4 rotate_y_deg(const mat4& m, float deg) {
	// convert to radians
	float rad = DEG2RAD(deg);
	mat4 m_r = identity_mat4();
	m_r.m[0] = cos(rad);
	m_r.m[8] = sin(rad);
	m_r.m[2] = -sin(rad);
	m_r.m[10] = cos(rad);
	return m_r * m;
}

// rotate around z axis by an angle in degrees
inline mat4 rotate_z_deg(const mat4& m, float deg) {
	// convert to radians
	float rad = DEG2RAD(deg);
	mat4 m_r = identity_mat4();
	m_r.m[0] = cos(rad);
	m_r.m[4] = -sin(rad);
	m_r.m[1] = sin(rad);
	m_r.m[5] = cos(rad);
	return m_r * m;
}

//Returns rotation matrix to rotate around axis u by a degrees
//from http://www.iquilezles.org/www/articles/noacos/noacos.htm
inline mat4 rotate_axis_deg(const vec3& u, float a){
	//Convert to radians
	float rad = DEG2RAD(a);

	float sin_a = sinf(rad);
	float cos_a = cosf(rad);
	float inv_cos_a = 1.0f - cos_a;

	return mat4(
		u.v[0]*u.v[0]*inv_cos_a + cos_a,
		u.v[0]*u.v[1]*inv_cos_a + sin_a*u.v[2],
		u.v[0]*u.v[2]*inv_cos_a - sin_a*u.v[1],
		0,
		u.v[1]*u.v[0]*inv_cos_a - sin_a*u.v[2],
		u.v[1]*u.v[1]*inv_cos_a + cos_a,
		u.v[1]*u.v[2]*inv_cos_a + sin_a*u.v[0],
		0,
		u.v[2]*u.v[0]*inv_cos_a + sin_a*u.v[1],
		u.v[2]*u.v[1]*inv_cos_a - sin_a*u.v[0],
		u.v[2]*u.v[2]*inv_cos_a + cos_a,
		0,
		0, 0, 0, 1
	);

}

//Returns rotation matrix to align u1 with u2 (MUST BE UNIT VECTORS)
//from http://www.iquilezles.org/www/articles/noacos/noacos.htm
inline mat4 rotate_align(const vec3& u1, const vec3& u2){
	vec3 axis = cross(u1,u2);
	float cos_a = dot(u1,u2);
    float k = 1.0f/(1.0f+cos_a);

	if(cmpf(cos_a,-1)) //vectors are opposite
	{
		//Rotate 180 degrees around axis u1 is least aligned with
		float dot_x = dot(u1,vec3(1,0,0));
		float dot_y = dot(u1,vec3(0,1,0));
		float dot_z = dot(u1,vec3(0,0,1));
		
		if(dot_y<dot_x){
			if(dot_z<dot_y) return rotate_z_deg(identity_mat4(), 180);
			else return rotate_y_deg(identity_mat4(), 180); 
		}
		else if(dot_z<dot_x) return rotate_z_deg(identity_mat4(), 180);
		else return rotate_x_deg(identity_mat4(), 180);
	}

	return mat4(
		axis.v[0]*axis.v[0]*k + cos_a,
		axis.v[0]*axis.v[1]*k + axis.v[2],
		axis.v[0]*axis.v[2]*k - axis.v[1],
		0,
		axis.v[1]*axis.v[0]*k - axis.v[2],
		axis.v[1]*axis.v[1]*k + cos_a,
		axis.v[1]*axis.v[2]*k + axis.v[0],
		0,
		axis.v[2]*axis.v[0]*k + axis.v[1],
		axis.v[2]*axis.v[1]*k - axis.v[0],
		axis.v[2]*axis.v[2]*k + cos_a,
		0,
		0, 0, 0, 1
	);
}

// scale a matrix by [x, y, z]
inline mat4 scale(const mat4& m, const vec3& v) {
	mat4 a = identity_mat4();
	a.m[0] = v.v[0];
	a.m[5] = v.v[1];
	a.m[10] = v.v[2];
	return a * m;
}

//scale a matrix uniformly by s
inline mat4 scale(const mat4& m, float s) {
	mat4 a = identity_mat4();
	a.m[0] = s;
	a.m[5] = s;
	a.m[10] = s;
	return a * m;
}

/*-----------------------VIRTUAL CAMERA MATRIX FUNCTIONS----------------------*/
// returns a view matrix using the opengl lookAt style. COLUMN ORDER.
inline mat4 look_at(const vec3& cam_pos, vec3 targ_pos, const vec3& up) {
	// inverse translation
	mat4 p = identity_mat4();
	p = translate(p, vec3(-cam_pos.v[0], -cam_pos.v[1], -cam_pos.v[2]));

	vec3 dist = targ_pos - cam_pos;
	vec3 fwd = normalise(dist);
	vec3 rgt = normalise(cross(fwd, up));
	vec3 up_act = normalise(cross(rgt, fwd));

	mat4 ori = identity_mat4();
	ori.m[0] = rgt.v[0];
	ori.m[4] = rgt.v[1];
	ori.m[8] = rgt.v[2];
	ori.m[1] = up_act.v[0];
	ori.m[5] = up_act.v[1];
	ori.m[9] = up_act.v[2];
	ori.m[2] = -fwd.v[0];
	ori.m[6] = -fwd.v[1];
	ori.m[10] = -fwd.v[2];
	/*  ori = {
			Rx Ux -Fx  0
			Ry Uy -Fy  0
			Rz Uz -Fz  0
			0  0   0   1
		}
		p = {
			1 0 0 -Px 
			0 1 0 -Py
			0 0 1 -Pz
			0 0 0  1
		}
	*/
	return ori * p;//p * ori;
}

//Returns an orthographic projection matrix
inline mat4 orthographic(float left, float right, float bottom, float top, float near_z, float far_z) {
	mat4 m = identity_mat4(); 
	m.m[0] = 2/(right-left);
	m.m[5] = 2/(top-bottom);
	m.m[10] = 2/(far_z-near_z);
	m.m[12] = -(right+left)/(right-left);
	m.m[13] = -(top+bottom)/(top-bottom);
	m.m[14] = (far_z+near_z)/(far_z-near_z);
	return m;
}

// returns a perspective function mimicking the opengl projection style.
inline mat4 perspective(float fovy, float aspect, float near_z, float far_z) {
	float fov_rad = DEG2RAD(fovy);
	float range = tan(fov_rad / 2.0f) * near_z;
	float sx = (2.0f * near_z) / (range * aspect + range * aspect);
	float sy = near_z / range;
	float sz = -(far_z + near_z) / (far_z - near_z);
	float pz = -(2.0f * far_z * near_z) / (far_z - near_z);
	mat4 m = zero_mat4(); // make sure bottom-right corner is zero
	m.m[0] = sx;
	m.m[5] = sy;
	m.m[10] = sz;
	m.m[14] = pz;
	m.m[11] = -1.0f;
	return m;
}

/*----------------------------HAMILTON IN DA HOUSE!---------------------------*/
inline versor quat_from_axis_rad(float radians, float x, float y, float z) {
	versor result;
	result.q[0] = cos(radians / 2.0);
	result.q[1] = sin(radians / 2.0) * x;
	result.q[2] = sin(radians / 2.0) * y;
	result.q[3] = sin(radians / 2.0) * z;
	return result;
}

inline versor quat_from_axis_deg(float degrees, float x, float y, float z) {
	return quat_from_axis_rad(DEG2RAD(degrees), x, y, z);
}

inline versor quat_from_axis_deg(float degrees, vec3 a) {
	return quat_from_axis_rad(DEG2RAD(degrees), a.v[0], a.v[1], a.v[2]);
}

inline mat4 quat_to_mat4(const versor& q) {
	float w = q.q[0];
	float x = q.q[1];
	float y = q.q[2];
	float z = q.q[3];
	return mat4(
		1.0f - 2.0f * y * y - 2.0f * z * z,
		2.0f * x * y + 2.0f * w * z,
		2.0f * x * z - 2.0f * w * y,
		0.0f,
		2.0f * x * y - 2.0f * w * z,
		1.0f - 2.0f * x * x - 2.0f * z * z,
		2.0f * y * z + 2.0f * w * x,
		0.0f,
		2.0f * x * z + 2.0f * w * y,
		2.0f * y * z - 2.0f * w * x,
		1.0f - 2.0f * x * x - 2.0f * y * y,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		1.0f
	);
}

inline versor normalise(versor& q) {
	// norm(q) = q / magnitude(q)
	// magnitude(q) = sqrt(w*w + x*x...)
	// only compute sqrt if interior sum != 1.0
	float sum =
		q.q[0] * q.q[0] + q.q[1] * q.q[1] +
		q.q[2] * q.q[2] + q.q[3] * q.q[3];
	// NB: floats have min 6 digits of precision
	const float thresh = 0.0001f;
	if(fabs(1.0f - sum) < thresh) {
		return q;
	}
	float mag = sqrt(sum);
	return q / mag;
}

inline float dot(const versor& q, const versor& r) {
	return q.q[0] * r.q[0] + q.q[1] * r.q[1] + q.q[2] * r.q[2] + q.q[3] * r.q[3];
}

inline versor slerp(versor& q, versor& r, float t) {
	// angle between q0-q1
	float cos_half_theta = dot(q, r);
	// as found here http://stackoverflow.com/questions/2886606/flipping-issue-when-interpolating-rotations-using-quaternions
	// if dot product is negative then one quaternion should be negated, to make
	// it take the short way around, rather than the long way
	// yeah! and furthermore Susan, I had to recalculate the d.p. after this
	if(cos_half_theta < 0.0f) {
		for(int i = 0; i < 4; i++) {
			q.q[i] *= -1.0f;
		}
		cos_half_theta = dot(q, r);
	}
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if(fabs(cos_half_theta) >= 1.0f) {
		return q;
	}
	// Calculate temporary values
	float sin_half_theta = sqrt(1.0f - cos_half_theta * cos_half_theta);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	versor result;
	if(fabs(sin_half_theta) < 0.001f) {
		for(int i = 0; i < 4; i++) {
			result.q[i] = (1.0f - t) * q.q[i] + t * r.q[i];
		}
		return result;
	}
	float half_theta = acos(cos_half_theta);
	float a = sin((1.0f - t) * half_theta) / sin_half_theta;
	float b = sin(t * half_theta) / sin_half_theta;
	for(int i = 0; i < 4; i++) {
		result.q[i] = q.q[i] * a + r.q[i] * b;
	}
	return result;
}

/*-----------------------------PRINT FUNCTIONS--------------------------------*/
inline void print(const vec2& v) {
	printf("[%.2f, %.2f]\n", v.v[0], v.v[1]);
}

inline void print(const vec3& v) {
	printf("[%.2f, %.2f, %.2f]\n", v.v[0], v.v[1], v.v[2]);
}

inline void print(const vec4& v) {
	printf("[%.2f, %.2f, %.2f, %.2f]\n", v.v[0], v.v[1], v.v[2], v.v[3]);
}

inline void print(const mat3& m) {
	printf("\n");
	printf("[%.2f][%.2f][%.2f]\n", m.m[0], m.m[3], m.m[6]);
	printf("[%.2f][%.2f][%.2f]\n", m.m[1], m.m[4], m.m[7]);
	printf("[%.2f][%.2f][%.2f]\n", m.m[2], m.m[5], m.m[8]);
}

inline void print(const mat4& m) {
	printf("\n");
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[0], m.m[4], m.m[8], m.m[12]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[1], m.m[5], m.m[9], m.m[13]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[2], m.m[6], m.m[10], m.m[14]);
	printf("[%.2f][%.2f][%.2f][%.2f]\n", m.m[3], m.m[7], m.m[11], m.m[15]);
}

inline void print(const versor& q) {
	printf("[%.2f ,%.2f, %.2f, %.2f]\n", q.q[0], q.q[1], q.q[2], q.q[3]);
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef __clang__
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif
