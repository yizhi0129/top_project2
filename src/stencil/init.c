#include "stencil/init.h"

#include "stencil/comm_handler.h"
#include "stencil/mesh.h"

#include <assert.h>
#include <math.h>

#include <omp.h>

static f64 compute_core_pressure(usz i, usz j, usz k) {
    return sin((f64)k * cos((f64)i + 0.311) * cos((f64)j + 0.817) + 0.613);
}

//single loop: may be more efficient but more indices to compute
static void setup_mesh_cell_values(mesh_t* mesh, comm_handler_t const* comm_handler) {
    usz ghost_size = 2 * STENCIL_ORDER;
    #pragma omp parallel for
    for (usz index = 0; index < (mesh->dim_x - ghost_size) * (mesh->dim_y - ghost_size) * (mesh->dim_z - ghost_size); ++index) {
        usz i = (index / ((mesh->dim_y - ghost_size) * (mesh->dim_z - ghost_size))) + STENCIL_ORDER;
        usz j = ((index / (mesh->dim_z - ghost_size)) % (mesh->dim_y - ghost_size)) + STENCIL_ORDER;
        usz k = (index % (mesh->dim_z - ghost_size)) + STENCIL_ORDER;

        usz ind = i * mesh->dim_y * mesh->dim_z + j * mesh->dim_z + k;
        switch (mesh->kind) {
            case MESH_KIND_CONSTANT:
                mesh->cells[ind].value = compute_core_pressure(
                    comm_handler->coord_x + i - STENCIL_ORDER,
                    comm_handler->coord_y + j - STENCIL_ORDER,
                    comm_handler->coord_z + k - STENCIL_ORDER
                );
                break;
            case MESH_KIND_INPUT:
                if ((i >= STENCIL_ORDER && (i < mesh->dim_x - STENCIL_ORDER)) &&
                    (j >= STENCIL_ORDER && (j < mesh->dim_y - STENCIL_ORDER)) &&
                    (k >= STENCIL_ORDER && (k < mesh->dim_z - STENCIL_ORDER)))
                {
                    mesh->cells[ind].value = 1.0;
                } else {
                    mesh->cells[ind].value = 0.0;
                }
                break;
            case MESH_KIND_OUTPUT:
                mesh->cells[ind].value = 0.0;
                break;
            default:
                __builtin_unreachable();
        }
    }
}

static void setup_mesh_cell_kinds(mesh_t* mesh) {
    usz ghost_size = 2 * STENCIL_ORDER;
    #pragma omp parallel for
    for (usz index = 0; index < (mesh->dim_x - ghost_size) * (mesh->dim_y - ghost_size) * (mesh->dim_z - ghost_size); ++index) {
        usz i = (index / ((mesh->dim_y - ghost_size) * (mesh->dim_z - ghost_size))) + STENCIL_ORDER;
        usz j = ((index / (mesh->dim_z - ghost_size)) % (mesh->dim_y - ghost_size)) + STENCIL_ORDER;
        usz k = (index % (mesh->dim_z - ghost_size)) + STENCIL_ORDER;

        usz ind = i * mesh->dim_y * mesh->dim_z + j * mesh->dim_z + k;
        mesh->cells[ind].kind = mesh_set_cell_kind(mesh, i, j, k);
    }
}


void init_meshes(mesh_t* A, mesh_t* B, mesh_t* C, comm_handler_t const* comm_handler) {
    assert(
        A->dim_x == B->dim_x && B->dim_x == C->dim_x &&
        C->dim_x == comm_handler->loc_dim_x + STENCIL_ORDER * 2
    );
    assert(
        A->dim_y == B->dim_y && B->dim_y == C->dim_y &&
        C->dim_y == comm_handler->loc_dim_y + STENCIL_ORDER * 2
    );
    assert(
        A->dim_z == B->dim_z && B->dim_z == C->dim_z &&
        C->dim_z == comm_handler->loc_dim_z + STENCIL_ORDER * 2
    );

    setup_mesh_cell_kinds(A);
    setup_mesh_cell_kinds(B);
    setup_mesh_cell_kinds(C);

    setup_mesh_cell_values(A, comm_handler);
    setup_mesh_cell_values(B, comm_handler);
    setup_mesh_cell_values(C, comm_handler);
}