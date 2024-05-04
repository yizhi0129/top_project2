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
    
    #pragma omp parallel for
    for (int i = 1; i <= STENCIL_ORDER; i++) {
        precomputed_powers[i] = pow(17.0, (f64)i);
    }

    usz ghost_size = 2 * STENCIL_ORDER;
    usz cell_count = A->dim_x * A->dim_y * A->dim_z;
    
    #pragma omp parallel for
    for (usz ind = 0; ind < cell_count; ++ind) {
        usz i = (ind / ((A->dim_y - ghost_size) * (A->dim_z - ghost_size))) + STENCIL_ORDER;
        usz j = ((ind / (A->dim_z - ghost_size)) % (A->dim_y - ghost_size)) + STENCIL_ORDER;
        usz k = (ind % (A->dim_z - ghost_size)) + STENCIL_ORDER;

        if (A->cells[ind].kind != CELL_KIND_CORE) {
            continue;
        }

        f64 sum = idx_const(A, i, j, k) * idx_const(B, i, j, k);

        for (usz o = 1; o <= STENCIL_ORDER; ++o) {
            sum += (
                (idx_const(A, i + o, j, k) * idx_core_const(B, i + o, j, k)) +
                (idx_const(A, i - o, j, k) * idx_core_const(B, i - o, j, k)) +
                (idx_const(A, i, j + o, k) * idx_core_const(B, i, j + o, k)) +
                (idx_const(A, i, j - o, k) * idx_core_const(B, i, j - o, k)) +
                (idx_const(A, i, j, k + o) * idx_core_const(B, i, j, k + o)) +
                (idx_const(A, i, j, k - o) * idx_core_const(B, i, j, k - o))
            ) / precomputed_powers[o];
        }
        *idx_core(C, i, j, k) = sum;
    }
    // Copy results back from C to A to prepare for the next iteration
    mesh_copy_core(A, C);
}