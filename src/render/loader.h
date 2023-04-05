#ifndef _loader_h_
#define _loader_h_

#include "rentypes.h"

vao_t* loader_load_vao(float* positions, float* tcoords, float* normals, unsigned int* indices, unsigned int n_vertices, unsigned int n_indices);

void loader_free_vao(vao_t* vao);

#endif