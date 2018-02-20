#pragma once
#include <stdint.h>

//****************************************
//Kevin's wavefront obj loading functions
//****************************************

//----------------------------------------------------------------------------------------------------------------------
//Load unindexed meshes (with/without UVs and normals)
bool load_obj(const char* file_name, 
			  float**     vp, 
			  uint32_t*   vert_count
			  );
bool load_obj(const char* file_name, 
			  float**     vp, 
			  float**     vt, 
			  float**     vn, 
			  uint32_t*   vert_count
			  );

//Load indexed meshes (with/without UVs and normals)
bool load_obj_indexed(const char* file_name, 
					  float**     vp, 
					  uint16_t**  indices, 
					  uint32_t*   vert_count, 
					  uint32_t*   index_count
					  );
bool load_obj_indexed(const char* file_name, 
					  float**     vp, 
					  float**     vt, 
					  float**     vn, 
					  uint16_t**  indices, 
					  uint32_t*   vert_count, 
					  uint32_t*   index_count, 
					  float       smooth_normal_factor=0.5 // (from 0-1) factor to decide if 2 vertices with different 
					  );                                   // normals should be treated as the same vert with an averaged
					                                       // norm or as two separate vertices;
                                                           // if dot(n1, n2) > factor then v1 and v2 are the same
					                                       // i.e. factor=cos(theta) means smooth normals if angle between faces is > theta
		                                                   // 0 is always smooth normals, 1 is never smooth normals
//----------------------------------------------------------------------------------------------------------------------
