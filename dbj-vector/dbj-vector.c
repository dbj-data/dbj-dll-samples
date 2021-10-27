
#include "dbj-vector.h"
#include "../dbj-component.h"

#include "vector.h"

/* --------------------------------------------------------------------------------- */
// dbj_component_version() generated here
DBJ_COMPONENT_VERSION_IMPLEMENTATION(0, 1, 0, "dbj vector component");
/* --------------------------------------------------------------------------------- */
// dbj_component_can_unload_now() is part of dbj-component definition
// it is also generated here
DBJ_COMPONENT_UNLOADER_IMPLEMENTATION;
/* --------------------------------------------------------------------------------- */
static struct dbj_vector_
    interface_implementation_ = {
        .create = create_vector,
        .destroy = destroy_vector,
        .size = vector_size,
        .capacity = vector_capacity,
        .push = vector_push,
        .insert = vector_insert,
        .split = vector_split,
        .at_c = vector_at_c,
        .at = vector_at,
        .swap = vector_swap,
        .clear = vector_clear,
        .erase = vector_erase};
/*
name is mandatory as mentioned in a def file
return type is intreface struct pointer
*/
struct dbj_vector_ *interface_factory(void)
{
    return &interface_implementation_;
}