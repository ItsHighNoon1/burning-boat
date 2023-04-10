#ifndef _render_h_
#define _render_h_

#include "loader.h"

void render_prepare();

void render_vao(vao_t* vao);

void render_bind_texture(texture_t* texture);

#endif