#include "stencil/solve.h"
#include <assert.h>
#include <math.h>

#include <omp.h>

void solve_jacobi(mesh_t* A, const mesh_t* B, mesh_t* C) {
    assert(A->dim_x == B->dim_x && B->dim_x == C->dim_x);
    assert(A->dim_y == B->dim_y && B->dim_y == C->dim_y);
    assert(A->dim_z == B->dim_z && B->dim_z == C->dim_z);

    omp_set_num_threads(24);

    f64 precomputed_powers[STENCIL_ORDER + 1];
    precomputed_powers[0] = 1.0;
    #pragma omp parallel for
    for (int i = 1; i <= STENCIL_ORDER; i++) {
        precomputed_powers[i] = pow(17.0, (f64)i);
    }

    usz ghost_size = 2 * STENCIL_ORDER;
    usz cell_count = (A->dim_x - ghost_size) * (A->dim_y - ghost_size) * (A->dim_z - ghost_size);
    #pragma omp parallel for
    for (usz ind = 0; ind < cell_count; ++ind) {
        usz i = (ind / ((A->dim_y - ghost_size) * (A->dim_z - ghost_size))) + STENCIL_ORDER;
        usz j = ((ind / (A->dim_z - ghost_size)) % (A->dim_y - ghost_size)) + STENCIL_ORDER;
        usz k = (ind % (A->dim_z - ghost_size)) + STENCIL_ORDER;

        if (i >= STENCIL_ORDER && i < A->dim_x - STENCIL_ORDER &&
            j >= STENCIL_ORDER && j < A->dim_y - STENCIL_ORDER &&
            k >= STENCIL_ORDER && k < A->dim_z - STENCIL_ORDER) {
                
            *idx_core(C, i, j, k) = *idx_core(A, i, j, k) * idx_core_const(B, i, j, k);

            // Apply stencil operation using values from A and B
            for (usz o = 1; o <= STENCIL_ORDER; ++o) {
                *idx_core(C, i, j, k) += (
                        (*idx_core(A, i + o, j, k) * idx_core_const(B, i + o, j, k)) +
                        (*idx_core(A, i - o, j, k) * idx_core_const(B, i - o, j, k)) +
                        (*idx_core(A, i, j + o, k) * idx_core_const(B, i, j + o, k)) +
                        (*idx_core(A, i, j - o, k) * idx_core_const(B, i, j - o, k)) +
                        (*idx_core(A, i, j, k + o) * idx_core_const(B, i, j, k + o)) +
                        (*idx_core(A, i, j, k - o) * idx_core_const(B, i, j, k - o))
                    ) / precomputed_powers[o];    
            }
        }
    }
}
