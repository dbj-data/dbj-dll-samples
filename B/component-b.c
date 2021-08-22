#include "component-b.h"
//
#include <string.h>
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
static void connection_string_(
    struct component_b *self_,
    const unsigned len_, char retval[static len_])
{
    if (!self_)
        return;
    if (len_ < strlen(self_->data_))
        return;
    strncpy(retval, self_->data_, len_);
}

///////////////////////////////////////////////////////////////////////////////////
// private instance of the interface is the component implementation
static struct component_b
    componenet_implementation_ = {.data_ = "server=localhost;user=root;pasword=qwqwqwqw123;",
                                  .connection_string = connection_string_};

// each dbj component must have exported function
// with the name "dbj_component_get_implementation"
// see the def file in this folder
struct component_b *dbj_component_get_implementation(void)
{
    return &componenet_implementation_;
}