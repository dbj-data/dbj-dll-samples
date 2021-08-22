#ifndef COMPONENT_B_INC_
#define COMPONENT_B_INC_
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

struct component_b
{
    const char * data_;
    // notice we do not return pointer made inside component space
    // we use the one created in the caller space
    void (*connection_string)(struct component_b *, const unsigned , char [static 1]);
};

// each dbj component has this as an exported function
// thus we do not actually need this declaration
// in this header
// bool dbj_component_can_unload_now(void);

// each dbj component has this as an exported function
// notice we do not return void *
// we do not actually need this declaration
// in this header
// struct component_b *dbj_component_get_implementation(void);

#ifdef __cplusplus
} // extern "C" 
#endif // __cplusplus
#endif // COMPONENT_B_INC_