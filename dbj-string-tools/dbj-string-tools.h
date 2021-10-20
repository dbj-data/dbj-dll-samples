#pragma once

#include <errno.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//  make sure build script creates this dll for this component
#define DBJ_STRING_TOOLS_DLL_NAME "dbj-string-tools.dll"
    //
    // one dll implements one dbj component
    // one dbj component has one interface
    // interface is a struct with data and function pointers
    // the interface :

    // dbj strings and string tools work on arrays of chars, not char pointers
    // thus the opportunity to bound them to some same value has been used
    enum
    {
        dbj_string_tools_max_input_count = 0xFF
    };

    struct dbj_string_tools
    {
        // rezult is array of 'word begin'/'word end' location pairs
        errno_t (*tokenizer)(
            // string of tokens
            const unsigned, const char[],
            // input string
            const unsigned, const char[],
            // result array structure:
            // zero elements: no of words found (0 on error)
            // then pairs of words, begin and end locations
            // then zeroes to the end of array
            // on bad input no of words found is 0 and errno == EINVAL
            const unsigned, unsigned /* the rezult array */[]);
    };

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
    // and for this component it is:
    typedef struct dbj_string_tools *(*dbj_string_utils_factory_fp)(void);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
