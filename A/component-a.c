#include "component-a.h"
//
#include <intrin.h>

static volatile long component_counter_ = 0;
//
__attribute__((constructor)) static void component_ctor(void)
{
    _InterlockedIncrement(&component_counter_);
}

__attribute__((destructor)) static void component_dtor()
{
    _InterlockedDecrement(&component_counter_);
}
// each dbj component has this as an exported function
bool dbj_component_can_unload_now(void)
{
    return component_counter_ == 0;
}


// specimen function definition
// best kept private to the dll
static int component_a_function_(struct component_a *self_)
{
    return self_->data_;
}

///////////////////////////////////////////////////////////////////////////////////
// private instance of the interface is the component implementation
static struct component_a componenet_implementation_ = { .data_ = 42, .function = component_a_function_ } ;


// each dbj component must have exported function
// with the name "dbj_component_get_implementation"
// see the def file in this folder
struct component_a *dbj_component_get_implementation(void)
{
    return & componenet_implementation_ ;
}