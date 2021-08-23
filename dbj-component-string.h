#ifndef DBJ_COMPONENT_STRING_INCLUDED
#define DBJ_COMPONENT_STRING_INCLUDED
/*
one dbj component == one dll == one component interface == one struct

   do not return pointers to the component internals, component can be unloaded any time
   dbj component string is an char array in a struct, for example

   struct { char data[0xFF]; } str1 ;

   if size is runtime value that is VLA and the resulting instance can not be initialized
   this is static thus free-ing is not required
   this can be sent into and out of functions and dbj components, 
   C runtime will do very fast copy elision in any case

   obviously wchar_t variant can and will be added as soon as it is required
*/

#include "dbj_capi/ccommon.h"
#include <string.h>

DBJ_EXTERN_C_BEGIN

#define DBJ_COMPONENT_STRING_TYPE(SIZE_) \
struct dbj_component_string_##SIZE_ { char data[SIZE_]; } 

// type names defined like that can clash and will clash
// thus we will define few probably most common in here
typedef DBJ_COMPONENT_STRING_TYPE(1024) dbj_component_string_1024 ;
typedef DBJ_COMPONENT_STRING_TYPE( 512) dbj_component_string_512 ;
typedef DBJ_COMPONENT_STRING_TYPE( 256) dbj_component_string_256 ;
typedef DBJ_COMPONENT_STRING_TYPE( 128) dbj_component_string_128 ;
typedef DBJ_COMPONENT_STRING_TYPE(  64) dbj_component_string_64 ;


#define DBJ_COMPONENT_STRING(NAME_, SIZE_) \
    struct                                 \
    {                                      \
        char data[SIZE_];                  \
    } NAME_ // = {.size = SIZE_} <-- no can do for VLA, ie, if SIZE_ is a runtime value

#define DBJ_COMPONENT_STRING_SIZE(STR_) DBJ_COUNT_OF(STR_.data)

#ifdef __STDC_LIB_EXT1__
#define DBJ_COMPONENT_STRING_ASSIGN(STR_, S_)                          \
    do                                                                 \
    {                                                                  \
        strncpy_s(STR_.data, DBJ_COUNT_OF(STR_.data), S_, strlen(S_)); \
    } while (0)
#else
#define DBJ_COMPONENT_STRING_ASSIGN(STR_, S_)            \
    do                                                   \
    {                                                    \
        strncpy(STR_.data, S_, DBJ_COUNT_OF(STR_.data)); \
    } while (0)
#endif

// this is important
// variable can be in 3 states: exist, does not exist and exist but **empty**
// that is dbj::valstat related and is not that usual in programing languages
// but in e.g. databases it is a norm; field is always there but can be empty
// we declare empty dbj component string the one that has '\0' as the first char

#define DBJ_COMPONENT_STRING_SET_EMPTY(STR_) ( STR_.data[0] = '\0' )

#define DBJ_COMPONENT_STRING_IS_EMPTY(STR_) ( STR_.data[0] == '\0' )

DBJ_EXTERN_C_END

#endif // DBJ_COMPONENT_STRING_INCLUDED