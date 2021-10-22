
/* --------------------------------------------------------------------------------- 
(c) 2021 by dbj@dbj.org https://license_dbj
*/
#include "src/decimal_from.h"   // this is not the public header!
#include "src/dbj_itoa_inner.h" // this is not the public header!

#include "../dbj-component.h"
#include "dbj-itoa.h" // dll interface

#include <string.h>

// dbj_component_version() generated here
DBJ_COMPONENT_VERSION_IMPLEMENTATION(0, 2, 0, "dbj itoa various forms");
/* --------------------------------------------------------------------------------- */
// dbj_component_can_unload_now() is part of dbj-component definition
// one usualy used the macro DBJ_COMPONENT_UNLOADER_IMPLEMENTATION
// to generate the implementation, or one can do it himself
// remember: we use only clang C and we work only on Windows
#include <intrin.h>

static volatile long component_counter_ = 0;

__attribute__((constructor)) static void component_ctor(void)
{
    _InterlockedIncrement(&component_counter_);
}

__attribute__((destructor)) static void component_dtor()
{
    _InterlockedDecrement(&component_counter_);
}

bool dbj_component_can_unload_now(void)
{
    return component_counter_ == 0;
}
/* --------------------------------------------------------------------------------- */
//  private instance of the interface implementation
/* --------------------------------------------------------------------------------- */
static dbj_itoa implementation_ = {
    // here connect function pointers of the public interface
    // to the private implementations
    // thus, both sides have to exhibit matching signatures
    .decimal_from = dbj_decimal_from,

    .ilog_10 = ilog_10,

    .lllog_10 = lllog_10,

    .ustoa = ustoa,

    .stoa = stoa,

    .uitoa = uitoa,

    .uitoa2 = uitoa2,

    .dbj_itoa = itoa_dbj,

    .ulltoa = ulltoa,

    .ulltoa2 = ulltoa2,

    .ulltoa3 = ulltoa3,

    .ulltoa4 = ulltoa4,

    .lltoa = lltoa,

    .dtoa = dtoa
    // eof implementation_
};

// each dbj dll aka "component"
// must have exported factory function
// with the name "interface_factory"
// see the def file in this folder
dbj_itoa *interface_factory(void)
{
    return &implementation_;
}