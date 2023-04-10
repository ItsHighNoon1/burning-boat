#include "sim.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <cglm/cglm.h>

// https://www.cs.cornell.edu/~bindel/class/cs5220-f11/code/sph.pdf

void _compute_densities(sim_t* sim) {
    float h2 = sim->h * sim->h;
    float h8 = (h2 * h2) * (h2 * h2);
    float c = 4.0f * sim->mass / GLM_PI / h8;
    for (unsigned int i = 0; i < sim->n_particles; i++) {
        sim->particles[i].rho = 4.0f * sim->mass / GLM_PI / h2;
        for (unsigned int j = i + 1; j < sim->n_particles; j++) {
            float r2 = glm_vec3_distance2(sim->particles[i].x, sim->particles[j].x);
            float z = h2 - r2;
            if (z > 0) {
                float rho_ij = c * z * z * z;
                sim->particles[i].rho += rho_ij;
                sim->particles[j].rho += rho_ij;
            }
        }
    }
}

sim_t* sim_create(unsigned int n_particles) {
    // Allocate simulation
    sim_t* sim = malloc(sizeof(sim_t));
    sim->particles = malloc(n_particles * sizeof(particle_t));
    sim->n_particles = n_particles;

    // Set initial state
    time_t t;
    srand((unsigned) time(&t));
    for (unsigned int i = 0; i < n_particles; i++) {
        sim->particles[i].x[0] = (float)rand() / (float)RAND_MAX;
        sim->particles[i].x[1] = (float)rand() / (float)RAND_MAX;
        sim->particles[i].x[2] = (float)rand() / (float)RAND_MAX;
        sim->particles[i].vh[0] = 0.0f;
        sim->particles[i].vh[1] = 0.0f;
        sim->particles[i].vh[2] = 0.0f;
        sim->particles[i].v[0] = 0.0f;
        sim->particles[i].v[1] = 0.0f;
        sim->particles[i].v[2] = 0.0f;
        sim->particles[i].a[0] = 0.0f;
        sim->particles[i].a[1] = 0.0f;
        sim->particles[i].a[2] = 0.0f;
    }

    return sim;
}

void sim_step(sim_t* sim) {
    _compute_densities(sim);
}

void sim_free(sim_t* sim) {
    free(sim->particles);
    free(sim);
}