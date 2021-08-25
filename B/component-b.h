#ifndef COMPONENT_B_INC_
#define COMPONENT_B_INC_

#include "../dbj-component-string.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//  make sure build creates this dll for this component
#define COMPONENT_B_DLL_NAME "component-b.dll"

///////////////////////////////////////////////////////////////////////////////////
// one dll implements one component
// one component has one interface
// interface is a struct with data and function pointers
// the interface struct for this component
struct component_b
{
    const char * data_;
    // notice we do not return pointer made inside component space
    // dbj_component_string_1024 is: struct { char data [1024]}
    dbj_component_string_1024 (*connection_string)(struct component_b *);
};

// we do not actually need this declaration in this header
//
// struct component_b *dbj_component_factory(void);
//
// dbj_component_factory is the only of the three functions
// from the def file whose function pointer declaration is required
// because it is unique per each component
//
// FP of the dbj_component_factory
// for this component
typedef struct component_b *(*component_b_factory_fp)(void);

#ifdef __cplusplus
} // extern "C" 
#endif // __cplusplus
#endif // COMPONENT_B_INC_