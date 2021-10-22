#ifndef COMPONENT_A_INC_
#define COMPONENT_A_INC_
#include <dbj_capi/dbj-string.h>

DBJ_EXTERN_C_BEGIN

//  make sure build creates this dll for this component
#define COMPONENT_A_DLL_NAME "component-a.dll"
///////////////////////////////////////////////////////////////////////////////////
// one dll implements is one dbj component
// one dbj component has one interface
// interface is a struct with data and function pointers
// the interface specimen:

struct component_a
{
    int data_;
    int (*get42)(struct component_a *);

    // notice we do not return pointer made inside component space
    // dbj_string_1024 is: struct { char data [1024]}
    dbj_string_1024 (*connection_string)(struct component_a *);
};

// each DBJ COMPONENT has the same def file
//
// EXPORTS
// dbj_component_can_unload_now    PRIVATE
// interface_factory           PRIVATE
// dbj_component_version           PRIVATE
//
// functions in DLL are exported by name
// not by full declaration
// we do not actually need them declared in headers
// but you do need them declared as function pointers
// in order to be called from clients
//
// Only the interface_factory has
// function pointer unique per each component
// its declaration is not required just the
// function pointer, it is unique per component
typedef struct component_a *(*component_a_factory_fp)(void);

// interface_factory and dbj_component_version
// have the same foot print for each components
// their function pointers are declared in
// dbj-component.h

DBJ_EXTERN_C_END

#endif // COMPONENT_A_INC_