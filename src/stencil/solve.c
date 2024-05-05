#include "stencil/solve.h"
#include <assert.h>
#include <math.h>
#include <omp.h>

#define BLOCK_SIZE 32  
//#define BLOCK_SIZE 64

void solve_jacobi(mesh_t* A, const mesh_t* B, mesh_t* C) {
    assert(A->dim_x == B->dim_x && B->dim_x == C->dim_x);
    assert(A->dim_y == B->dim_y && B->dim_y == C->dim_y);
    assert(A->dim_z == B->dim_z && B->dim_z == C->dim_z);

    omp_set_num_threads(1);  
    //omp_set_num_threads(2);  
    //omp_set_num_threads(4);  
    //omp_set_num_threads(8);  
    //omp_set_num_threads(16);  
    //omp_set_num_threads(24);  
    //omp_set_num_threads(48);  

    f64 precomputed_powers[STENCIL_ORDER + 1];
    #pragma omp parallel for
    for (int i = 1; i <= STENCIL_ORDER; i++) {
        precomputed_powers[i] = pow(17.0, (f64)i);
    }

    usz cell_count = A->dim_x * A->dim_y * A->dim_z;

    #pragma omp parallel for collapse(3) schedule(dynamic)
    for (usz bi = 0; bi < A->dim_x; bi += BLOCK_SIZE) {
        for (usz bj = 0; bj < A->dim_y; bj += BLOCK_SIZE) {
            for (usz bk = 0; bk < A->dim_z; bk += BLOCK_SIZE) {
                for (usz i = bi; i < bi + BLOCK_SIZE && i < A->dim_x; i++) {
                    for (usz j = bj; j < bj + BLOCK_SIZE && j < A->dim_y; j++) {
                        for (usz k = bk; k < bk + BLOCK_SIZE && k < A->dim_z; k++) {
                            if (A->cells[i * A->dim_y * A->dim_z + j * A->dim_z + k].kind != CELL_KIND_CORE) {
                                continue;
                            }
                            f64 sum = idx_const(A, i, j, k) * idx_const(B, i, j, k);
                            for (usz o = 1; o <= STENCIL_ORDER; ++o) {
                                sum += (
                                    (idx_const(A, i + o, j, k) * idx_const(B, i + o, j, k)) +
                                    (idx_const(A, i - o, j, k) * idx_const(B, i - o, j, k)) +
                                    (idx_const(A, i, j + o, k) * idx_const(B, i, j + o, k)) +
                                    (idx_const(A, i, j - o, k) * idx_const(B, i, j - o, k)) +
                                    (idx_const(A, i, j, k + o) * idx_const(B, i, j, k + o)) +
                                    (idx_const(A, i, j, k - o) * idx_const(B, i, j, k - o))
                                ) / precomputed_powers[o];
                            }
                            *idx(C, i, j, k) = sum;
                        }
                    }
                }
            }
        }
    }
    mesh_copy_core(A, C);
}
