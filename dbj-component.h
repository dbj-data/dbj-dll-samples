#ifndef DBJ_COMPONENT_INC_
#define DBJ_COMPONENT_INC_
#ifndef __clang__
#error __FILE__ " No time for MSVC shenanigans ..."
#endif // __clang__
#include <stdbool.h>

///////////////////////////////////////////////////////////////////////////////////
// one dll implements one component
// one component has one interface
// interface is a struct with data and function pointers
// interface is known and public knowledge
// to the users of dbj-component 
// the interface specimen example
struct specimen_
{
    int data_;
    int (*function)(struct specimen_ *);
};

// each dbj component has this as an exported function
__attribute__((overloadable)) bool dbj_component_can_unload_now(void);

// each dbj component has this as an exported function
// notice we do not return void *
__attribute__((overloadable)) struct specimen_ *dbj_component_get_implementation(void);

#endif // DBJ_COMPONENT_INC_