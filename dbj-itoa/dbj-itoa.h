#pragma once

#include <errno.h>

//  make sure build script creates this dll for this component
#define DBJ_STRING_TOOLS_DLL_NAME "dbj-itoa.dll"

// the one and only interface

enum
{
    dbj_itoa_min_buffer_count = 0xF
};

#ifndef SAFE_BUF_LEN
#define SAFE_BUF_LEN 0xF
#endif // ! SAFE_BUF_LEN

typedef struct dbj_itoa_ dbj_itoa;
struct dbj_itoa_
{
    // right now there is no error signaling whatsoever
    // roadmap is to return NULL on error
    char const *(*decimal_from)(long /* input */, const unsigned, char[]);

    unsigned int (*ilog_10)(unsigned int);

    unsigned int (*lllog_10)(unsigned long long);

    char *(*ustoa)(char[static SAFE_BUF_LEN], unsigned short);

    char *(*stoa)(char[static SAFE_BUF_LEN], short);

    char *(*uitoa)(char[static SAFE_BUF_LEN], unsigned int);

    char *(*uitoa2)(char[static SAFE_BUF_LEN], unsigned int);

    char *(*dbj_itoa)(char[static SAFE_BUF_LEN], int);

    char *(*ulltoa)(char[static SAFE_BUF_LEN], unsigned long long);

    char *(*ulltoa2)(char[static SAFE_BUF_LEN], unsigned long long);

    char *(*ulltoa3)(char[static SAFE_BUF_LEN], unsigned long long);

    char *(*ulltoa4)(char[static SAFE_BUF_LEN], unsigned long long);

    char *(*lltoa)(char[static SAFE_BUF_LEN], long long);

    char *(*dtoa)(char[static SAFE_BUF_LEN], double);
};

typedef dbj_itoa *(*dbj_itoa_ifp)(void);

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
// struct dbj_strong *interface_factory(void)
// {
//     return &componenet_implementation_;
// }
//
// Of the three fuinctions from the def file,
// only the "interface_factory" has
// function pointer unique per each component
// its declaration is the function pointer
