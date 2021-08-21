#include "dbj-component.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static volatile long component_counter_ = 0;
//
__attribute__((constructor)) static void component_ctor(void)
{
    InterlockedIncrement(&component_counter_);
}

__attribute__((destructor)) static void component_dtor()
{
    InterlockedDecrement(&component_counter_);
}
// each dbj component has this as an exported function
bool dbj_component_can_unload_now(void)
{
    return component_counter_ == 0;
}


// specimen function definition
// best kept private to the dll
static int specimen_function_(struct specimen_ *self_)
{
    return self_->data_;
}

///////////////////////////////////////////////////////////////////////////////////
// private instance of the interface creates the component implementation
static struct specimen_ componenet_implementation_ = { .data_ = 42, .function = specimen_function_ } ;


// each dbj component has this as an exported function
struct specimen_ *dbj_component_get_implementation(void)
{
    return & componenet_implementation_ ;
}