#ifndef COMPONENT_A_INC_
#define COMPONENT_A_INC_
#ifndef __clang__
#error __FILE__ " No time for MSVC shenanigans ..."
#endif // __clang__
#include <stdbool.h>
#ifndef _WIN32
#error This is WIN32 code only
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

///////////////////////////////////////////////////////////////////////////////////
// one dll implements one component
// one component has one interface
// interface is a struct with data and function pointers
// interface is known and public knowledge
// to the users of dbj-component 
// the interface example

struct component_a
{
    int data_;
    int (*get42)(struct component_a *);
};

// each dbj component has this as an exported function
// thus we do not actually need this declaration
// in this header
// bool dbj_component_can_unload_now(void);

// each dbj component has this as an exported function
// notice we do not return void *
// we do not actually need this declaration
// in this header
// struct component_a *dbj_component_factory(void);

#ifdef __cplusplus
} // extern "C" 
#endif // __cplusplus
#endif // COMPONENT_A_INC_