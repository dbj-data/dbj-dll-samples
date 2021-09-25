#ifndef COMPONENT_A_INC_
#define COMPONENT_A_INC_

#include <dbj_capi/dbj-string.h>

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

    enum
    {
        dbj_string_tools_max_input_count = 0xFF
    };

    struct dbj_string_tools
    {
        unsigned (*tokenizer)(
            // string of tokens
            const unsigned /*token_len*/, const char /*token*/[/*token_len*/],
            // input string
            const unsigned /*input_len*/, const char /*input*/[/*input_len*/],
            // result array
            // zero element: no of word found
            // then pairs of words begin and end locations
            // then zeroes to the end of array
            // on bad input no of words found is 0 and errno == EINVAL
            const unsigned /*output_len*/, unsigned /*output*/[/*output_len*/]);
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
    // their function pointers are declared in
    // dbj-component.h
    //

    // factory function for this component is implemented as:
    //
    // struct dbj_strong *dbj_component_factory(void)
    // {
    //     return &componenet_implementation_;
    // }
    //
    // Only the "dbj_component_factory" has
    // function pointer unique per each component
    // its declaration is not required
    // the factroy function function pointer
    // unique for this component is:
    typedef struct dbj_string_tools *(*dbj_string_utils_factory_fp)(void);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // COMPONENT_A_INC_