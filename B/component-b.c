#include "component-b.h"
#include "../dbj-component.h"
#include "../dbj-component-string.h"
//
#include <string.h>
#include <intrin.h>

DBJ_COMPONENT_VERSION_IMPLEMENTATION(0,1,0,"dbj component B");
/* --------------------------------------------------------------------------------- */
// dbj_component_can_unload_now() is part of dbj-component definition
// it is also generted here
DBJ_COMPONENT_UNLOADER_IMPLEMENTATION ;
/* --------------------------------------------------------------------------------- */
// function definition
// kept private to the componenta
static dbj_component_string_1024 connection_string_(
    struct component_b *self_ )
{
    dbj_component_string_1024 retval = {{0}};

    if (!self_)
        return retval ;

    DBJ_COMPONENT_STRING_ASSIGN( retval, self_->data_) ;

    return retval ;
}

///////////////////////////////////////////////////////////////////////////////////
// private instance of the interface is the component implementation
static struct component_b
    componenet_implementation_ = {.data_ = "server=localhost;user=root;pasword=qwqwqwqw123;",
                                  .connection_string = connection_string_};

// each dbj component must have exported function
// with the name "dbj_component_factory"
// see the def file in this folder
struct component_b *dbj_component_factory(void)
{
    return &componenet_implementation_;
}