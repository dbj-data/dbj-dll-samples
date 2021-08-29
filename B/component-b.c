#include "component-b.h"
#include "../dbj-component.h"
#include "../dbj-component-string.h"
//
#include <string.h>
#include <intrin.h>

DBJ_COMPONENT_VERSION_IMPLEMENTATION(0, 1, 0, "dbj component B");
/* --------------------------------------------------------------------------------- */
// dbj_component_can_unload_now() is part of dbj-component definition
// it is also generted here
DBJ_COMPONENT_UNLOADER_IMPLEMENTATION;
/* --------------------------------------------------------------------------------- */
static HANDLE create_method(struct component_b *self_, dbj_component_string_64 key_, unsigned size_ )
{
    return 0;
}
static HANDLE delete_method(struct component_b *self_, HANDLE handle_)
{
    return 0;
}

static bool set_value_method(struct component_b *self_, HANDLE handle_, void *value)
{
    return true;
}
static bool get_value_method(struct component_b *self_, HANDLE handle_, void **value)
{
    return true;
}
///////////////////////////////////////////////////////////////////////////////////
// private instance of the interface is the component implementation
static struct component_b
    componenet_implementation_ =
        {
            .create = create_method,
            .delete = delete_method,
            .get_value = get_value_method,
            .set_value = set_value_method
        };

// each dbj component must have exported function
// with the name "dbj_component_factory"
// see the def file in this folder
struct component_b *dbj_component_factory(void)
{
    return &componenet_implementation_;
}