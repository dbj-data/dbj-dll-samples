#pragma once

#include <errno.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//  make sure build script creates this dll for this component
#define DBJ_STRING_TOOLS_DLL_NAME "dbj-itoa.dll"

    // the one and only interface

    enum
    {
        dbj_itoa_min_buffer_count = 0xFF
    };

    struct dbj_itoa
    {
        // right now there is no error signaling whatsoever
        // roadmap is to return NULL on error
        char const *(*decimal_from)(long /* input */, const unsigned, char[]);
    };

    typedef struct dbj_itoa *(*dbj_itoa_factory_fp)(void);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

// each DBJ COMPONENT has the same def file
//
// EXPORTS
// dbj_component_can_unload_now    PRIVATE
// dbj_component_factory           PRIVATE
// dbj_component_version           PRIVATE
//
// functions in DLL are exported by name
// not by full declaration
// we do not actually need them declared in headers
// but clients do need them declared as function pointers
//
// 'dbj_component_can_unload_now' and 'dbj_component_version'
// have the same foot print for each components
// in case you want to use them
// their function pointers are declared in
// dbj-component.h
//

// factory function for this component is implemented
// in the C file as:
//
// struct dbj_strong *dbj_component_factory(void)
// {
//     return &componenet_implementation_;
// }
//
// Of the three fuinctions from the def file,
// only the "dbj_component_factory" has
// function pointer unique per each component
// its declaration is the function pointer
