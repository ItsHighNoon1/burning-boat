#ifndef _sim_h_
#define _sim_h_

#include <cglm/cglm.h>

#include "../render/shader.h"
#include "../util/loader.h"

typedef struct {
    mat4 model_view_matrix;
    vec3 x;
    float rho;
    vec3 vh;
    float pad1;
    vec3 v;
    float pad2;
    vec3 a;
    float pad3;
} particle_t;

typedef struct {
    particle_t* particles;
    unsigned int n_particles;
    float h;
    float dt;
    float rho0;
    float k;
    float mu;
    float mass;
    vec3 g;
    shader_t* density_compute;
    shader_t* acceleration_compute;
    shader_t* integration_compute;
    ssbo_t* ssbo;
    int location_n_particles;
    int location_h;
    int location_mass;
} sim_t;

sim_t* sim_create(unsigned int n_particles);

void sim_step(sim_t* sim);

void sim_free(sim_t* sim);

#endif