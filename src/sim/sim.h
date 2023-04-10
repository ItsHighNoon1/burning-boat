#ifndef _sim_h_
#define _sim_h_

#include <cglm/cglm.h>

typedef struct {
    float rho;
    vec3 x;
    vec3 vh;
    vec3 v;
    vec3 a;
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
} sim_t;

sim_t* sim_create(unsigned int n_particles);

void sim_step(sim_t* sim);

void sim_free(sim_t* sim);

#endif