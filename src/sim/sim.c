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
        sim->particles[i].rho = 0.0f;
    }
    for (unsigned int i = 0; i < sim->n_particles; i++) {
        sim->particles[i].rho += 4.0f * sim->mass / GLM_PI / h2;
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

void _compute_accelerations(sim_t* sim) {
    _compute_densities(sim);

    float h2 = sim->h * sim->h;

    for (unsigned int i = 0; i < sim->n_particles; i++) {
        glm_vec3_copy(sim->g, sim->particles[i].a);
    }

    // ???
    float c0 = sim->mass / GLM_PI / (h2 * h2);
    float cp = 15.0f * sim->k;
    float cv = -40.0f * sim->mu;

    for (unsigned int i = 0; i < sim->n_particles; i++) {
        for (unsigned int j = i + 1; j < sim->n_particles; j++) {
            vec3 position_diff;
            glm_vec3_sub(sim->particles[i].x, sim->particles[j].x, position_diff);
            float r2 = glm_vec3_norm2(position_diff);
            if (r2 < h2) {
                float q = sqrtf(r2) / sim->h;
                float u = 1.0f - q;
                float w0 = c0 * u / sim->particles[i].rho / sim->particles[j].rho;
                float wp = w0 * cp * (sim->particles[i].rho + sim->particles[j].rho - 2.0f * sim->rho0) * u / q;
                float wv = w0 * cv;

                vec3 velocity_diff;
                glm_vec3_sub(sim->particles[i].v, sim->particles[j].v, velocity_diff);
                glm_vec3_scale(position_diff, wp, position_diff);
                glm_vec3_scale(velocity_diff, wv, velocity_diff);
                glm_vec3_negate(position_diff);
                glm_vec3_negate(velocity_diff);
                glm_vec3_add(sim->particles[i].a, position_diff, sim->particles[i].a);
                glm_vec3_add(sim->particles[i].a, velocity_diff, sim->particles[i].a);
                glm_vec3_negate(position_diff);
                glm_vec3_negate(velocity_diff);
                glm_vec3_add(sim->particles[j].a, position_diff, sim->particles[j].a);
                glm_vec3_add(sim->particles[j].a, velocity_diff, sim->particles[j].a);
            }
        }
    }
}

void _reflect_bc(sim_t* sim) {
    for (unsigned int i = 0; i < sim->n_particles; i++) {
        if (sim->particles[i].x[0] > 2.5f) {
            sim->particles[i].x[0] = 2.5f;
            sim->particles[i].v[0] = -sim->particles[i].v[0];
            sim->particles[i].vh[0] = -sim->particles[i].vh[0];
            glm_vec3_scale(sim->particles[i].v, 0.5f, sim->particles[i].v);
            glm_vec3_scale(sim->particles[i].vh, 0.5f, sim->particles[i].vh);
        }
        if (sim->particles[i].x[0] < -2.5f) {
            sim->particles[i].x[0] = -2.5f;
            sim->particles[i].v[0] = -sim->particles[i].v[0];
            sim->particles[i].vh[0] = -sim->particles[i].vh[0];
            glm_vec3_scale(sim->particles[i].v, 0.5f, sim->particles[i].v);
            glm_vec3_scale(sim->particles[i].vh, 0.5f, sim->particles[i].vh);
        }
        if (sim->particles[i].x[2] > 2.5f) {
            sim->particles[i].x[2] = 2.5f;
            sim->particles[i].v[2] = -sim->particles[i].v[2];
            sim->particles[i].vh[2] = -sim->particles[i].vh[2];
            glm_vec3_scale(sim->particles[i].v, 0.5f, sim->particles[i].v);
            glm_vec3_scale(sim->particles[i].vh, 0.5f, sim->particles[i].vh);
        }
        if (sim->particles[i].x[2] < -2.5f) {
            sim->particles[i].x[2] = -2.5f;
            sim->particles[i].v[2] = -sim->particles[i].v[2];
            sim->particles[i].vh[2] = -sim->particles[i].vh[2];
            glm_vec3_scale(sim->particles[i].v, 0.5f, sim->particles[i].v);
            glm_vec3_scale(sim->particles[i].vh, 0.5f, sim->particles[i].vh);
        }
        if (sim->particles[i].x[1] < 0.0f) {
            sim->particles[i].x[1] = 0.0f;
            sim->particles[i].v[1] = -sim->particles[i].v[1];
            sim->particles[i].vh[1] = -sim->particles[i].vh[1];
            glm_vec3_scale(sim->particles[i].v, 0.5f, sim->particles[i].v);
            glm_vec3_scale(sim->particles[i].vh, 0.5f, sim->particles[i].vh);
        }
    }
}

void _leapfrog_step(sim_t* sim) {
    for (unsigned int i = 0; i < sim->n_particles; i++) {
        vec3 scaled_a;
        vec3 scaled_a2;
        vec3 scaled_vh;
        glm_vec3_scale(sim->particles[i].a, sim->dt, scaled_a);
        glm_vec3_scale(scaled_a, 0.5f, scaled_a2);
        glm_vec3_add(sim->particles[i].vh, scaled_a, sim->particles[i].vh);
        glm_vec3_add(sim->particles[i].vh, scaled_a2, sim->particles[i].v);
        glm_vec3_scale(sim->particles[i].vh, sim->dt, scaled_vh);
        glm_vec3_add(sim->particles[i].x, scaled_vh, sim->particles[i].x);
    }
    _reflect_bc(sim);
}

void _normalize_mass(sim_t* sim) {
    sim->mass = 1.0f;
    _compute_densities(sim);
    float rhos = 0.0f;
    float rho2s = 0.0f;
    for (unsigned int i = 0; i < sim->n_particles; i++) {
        rhos += sim->particles[i].rho;
        rho2s += sim->particles[i].rho * sim->particles[i].rho;
    }
    sim->mass *= sim->rho0 * rhos / rho2s;
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
        sim->particles[i].rho = 0.0f;
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
    sim->dt = 0.0016;
    sim->h = 5e-2f;
    sim->rho0 = 1000.0f;
    sim->k = 1e3f;
    sim->mu = 0.1f;
    sim->g[0] = 0.0f;
    sim->g[1] = -1.0f;
    sim->g[2] = 0.0f;
    _normalize_mass(sim);

    return sim;
}

void sim_step(sim_t* sim) {
    _compute_accelerations(sim);
    _leapfrog_step(sim);
}

void sim_free(sim_t* sim) {
    free(sim->particles);
    free(sim);
}