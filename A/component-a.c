#include "component-a.h"

#include "../dbj-component.h"
#include "../dbj-string.h"

#include <intrin.h>

// dbj_component_version() generated here
DBJ_COMPONENT_VERSION_IMPLEMENTATION(0,1,0,"dbj component A");
/* --------------------------------------------------------------------------------- */
// dbj_component_can_unload_now() is part of dbj-component definition
// it is also generated here
DBJ_COMPONENT_UNLOADER_IMPLEMENTATION ;
/* --------------------------------------------------------------------------------- */
// connection_string_ is connection_string functionimplementation
// kept private to the component
static dbj_string_1024 connection_string_(
    struct component_a *self_ )
{
    dbj_string_1024 retval = {{0}};

    if (!self_)
        return retval ;

    DBJ_STRING_ASSIGN( retval, "server=localhost;user=root;pasword=qwqwqwqw123;") ;

    return retval ;
}
/* --------------------------------------------------------------------------------- */

// specimen function definition
// best kept private to the dll
static int get42implementation(struct component_a *self_)
{
    if (self_)
       return self_->data_;

    return 0 ;   
}

///////////////////////////////////////////////////////////////////////////////////
// private instance of the interface is the component implementation
static struct component_a componenet_implementation_ 
= { .data_ = 42, .get42 = get42implementation, .connection_string = connection_string_ } ;


// each dbj component must have exported function
// with the name "dbj_component_factory"
// see the def file in this folder
struct component_a *dbj_component_factory(void)
{
    return & componenet_implementation_ ;
}