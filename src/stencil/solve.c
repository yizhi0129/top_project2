#include "stencil/solve.h"
#include <assert.h>
#include <math.h>

void solve_jacobi(mesh_t* A, const mesh_t* B, mesh_t* C) {
    assert(A->dim_x == B->dim_x && B->dim_x == C->dim_x);
    assert(A->dim_y == B->dim_y && B->dim_y == C->dim_y);
    assert(A->dim_z == B->dim_z && B->dim_z == C->dim_z);

    for (usz k = STENCIL_ORDER; k < C->dim_z - STENCIL_ORDER; ++k) {
        for (usz j = STENCIL_ORDER; j < C->dim_y - STENCIL_ORDER; ++j) {
            for (usz i = STENCIL_ORDER; i < C->dim_x - STENCIL_ORDER; ++i) {
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
                    ) / pow(17.0, (f64)o);
                }
            }
        }
    }
}
