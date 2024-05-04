#include "stencil/mesh.h"

#include "logging.h"

#include <assert.h>
#include <stdlib.h>

mesh_t mesh_new(usz dim_x, usz dim_y, usz dim_z, mesh_kind_t kind) {
    usz const ghost_size = 2 * STENCIL_ORDER;
    usz const count = (dim_x + ghost_size) * (dim_y + ghost_size) * (dim_z + ghost_size);
    cell_t* cells = malloc(count * sizeof(cell_t));
    if (NULL == cells) {
        error("failed to allocate mesh of size %zu bytes", count * sizeof(cell_t));
    }
    return (mesh_t){
        .dim_x = dim_x + ghost_size,
        .dim_y = dim_y + ghost_size,
        .dim_z = dim_z + ghost_size,
        .cells = cells,
        .kind = kind,
    };
} // return dimenstion including ghost cells

void mesh_drop(mesh_t* self) {
    if (NULL != self->cells) {
        free(self->cells);
        self->cells = NULL;
    }
}

static char const* mesh_kind_as_str(mesh_t const* self) {
    static char const* MESH_KINDS_STR[] = {
        "CONSTANT",
        "INPUT",
        "OUTPUT",
    };
    return MESH_KINDS_STR[(usz)self->kind];
}

void mesh_print(mesh_t const* self, char const* name) {
    fprintf(
        stderr,
        "****************************************\n"
        "MESH `%s`\n\tKIND: %s\n\tDIMS: %zux%zux%zu\n\tVALUES:\n",
        name,
        mesh_kind_as_str(self),
        self->dim_x,
        self->dim_y,
        self->dim_z
    );

    usz ghost_size = 2 * STENCIL_ORDER;
    usz cell_count = self->dim_x * self->dim_y * self->dim_z;
    for (usz index = 0; index < cell_count; ++index) {
        usz i = (index / ((self->dim_y - ghost_size) * (self->dim_z - ghost_size))) + STENCIL_ORDER;
        usz j = ((index / (self->dim_z - ghost_size)) % (self->dim_y - ghost_size)) + STENCIL_ORDER;
        usz k = (index % (self->dim_z - ghost_size)) + STENCIL_ORDER;

        printf(
            "%s%6.3lf%s ",
            CELL_KIND_CORE == mesh_set_cell_kind(self, i, j, k) ? "\x1b[1m" : "",
            idx_core_const(self, i, j, k),
            "\x1b[0m"
        );

        if ((index + 1) % (self->dim_z - ghost_size) == 0) {
            puts("");
        }
    }
    puts("");
}


cell_kind_t mesh_set_cell_kind(mesh_t const* self, usz i, usz j, usz k) {
    if ((i >= STENCIL_ORDER && i < self->dim_x - STENCIL_ORDER) &&
        (j >= STENCIL_ORDER && j < self->dim_y - STENCIL_ORDER) &&
        (k >= STENCIL_ORDER && k < self->dim_z - STENCIL_ORDER))
    {
        return CELL_KIND_CORE;
    } else {
        return CELL_KIND_PHANTOM;
    }
}

void mesh_copy_core(mesh_t* dst, mesh_t const* src) {
    assert(dst->dim_x == src->dim_x);
    assert(dst->dim_y == src->dim_y);
    assert(dst->dim_z == src->dim_z);

    usz ghost_size = 2 * STENCIL_ORDER;
    usz cell_count = (dst->dim_x - ghost_size) * (dst->dim_y - ghost_size) * (dst->dim_z - ghost_size);
    for (usz index = 0; index < cell_count; ++index) {
        usz dst_i = (index / ((dst->dim_y - ghost_size) * (dst->dim_z - ghost_size))) + STENCIL_ORDER;
        usz dst_j = ((index / (dst->dim_z - ghost_size)) % (dst->dim_y - ghost_size)) + STENCIL_ORDER;
        usz dst_k = (index % (dst->dim_z - ghost_size)) + STENCIL_ORDER;

        f64 value = idx_core_const(src, dst_i, dst_j, dst_k);
        *idx_core(dst, dst_i, dst_j, dst_k) = value;
    }
}



/// Returns a pointer to the indexed element (includes surrounding ghost cells).
f64* idx(mesh_t* self, usz i, usz j, usz k)
{
    usz index = i * self->dim_y * self->dim_z + j * self->dim_z + k;
    return &(self->cells[index].value);
}

/// Returns a pointer to the indexed element (ignores surrounding ghost cells).
f64* idx_core(mesh_t* self, usz i, usz j, usz k)
{
    usz index = (i - STENCIL_ORDER) * self->dim_y * self->dim_z 
                + (j - STENCIL_ORDER) * self->dim_z 
                + (k - STENCIL_ORDER);
    assert(index < (self->dim_x * self->dim_y * self->dim_z)); 
    return &(self->cells[index].value);
}

/// Returns the value at the indexed element (includes surrounding ghost cells).
f64 idx_const(mesh_t const* self, usz i, usz j, usz k)
{
    usz index = i * self->dim_y * self->dim_z + j * self->dim_z + k;
    return self->cells[index].value;
}

/// Returns the value at the indexed element (ignores surrounding ghost cells).
f64 idx_core_const(mesh_t const* self, usz i, usz j, usz k)
{
    usz index = (i - STENCIL_ORDER) * self->dim_y * self->dim_z 
                + (j - STENCIL_ORDER) * self->dim_z 
                + (k - STENCIL_ORDER);
    assert(index < (self->dim_x * self->dim_y * self->dim_z));   
    return self->cells[index].value;
}
