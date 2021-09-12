#ifndef DBJ_STRING_INCLUDED
#define DBJ_STRING_INCLUDED
/*
   dbj capi string is an char array in a struct, for example

   struct { char data[0xFF]; } str1 = {""};

   if size is runtime value the resulting instance can not be initialized

   int j = 0xFF ;
   struct { char data[j]; } str1 = {""}; // does not compile
   struct { char data[j]; } str1 ; // does compile

   this is static thus free-ing is not required
   this can be sent into and out of functions and dbj components, 
   C runtime will do very fast copy elision in any case

   obviously wchar_t variant can and will be added as soon as it is required
   same is for utf-8, utf-16 and utf-32

   Godbolt quick test: https://godbolt.org/z/MG7dhaq43
*/
// #define __STDC_WANT_LIB_EXT1__
#include <string.h>

#ifdef __cplusplus
#define DBJ_EXTERN_C_BEGIN \
    extern "C"             \
    {
#define DBJ_EXTERN_C_END }
#else // ! __cplusplus
#define DBJ_EXTERN_C_BEGIN
#define DBJ_EXTERN_C_END
#endif // !__cplusplus

DBJ_EXTERN_C_BEGIN

#undef DBJ_COUNT_OF
#define DBJ_COUNT_OF(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#define DBJ_STRING_TYPE(SIZE_) \
    struct dbj_string_##SIZE_  \
    {                          \
        char data[SIZE_];      \
    }

// type names defined like that can clash and will clash
// thus we will define few probably most common
typedef DBJ_STRING_TYPE(1024) dbj_string_1024;
typedef DBJ_STRING_TYPE(512) dbj_string_512;
typedef DBJ_STRING_TYPE(256) dbj_string_256;
typedef DBJ_STRING_TYPE(128) dbj_string_128;
typedef DBJ_STRING_TYPE(64) dbj_string_64;
typedef DBJ_STRING_TYPE(32) dbj_string_32;
typedef DBJ_STRING_TYPE(16) dbj_string_16;

/*
anonymous struct used thus no name clashes
*/
#define DBJ_STRING(NAME_, SIZE_) \
    struct                       \
    {                            \
        char data[SIZE_];        \
    } NAME_ // = {.size = SIZE_} <-- no can do for VLA, ie, if SIZE_ is a runtime value

#define DBJ_STRING_SIZE(STR_) DBJ_COUNT_OF(STR_.data)

#ifdef _MSC_VER
// #ifdef __STDC_LIB_EXT1__
#define DBJ_STRING_ASSIGN(STR_, S_)                                                     \
    do                                                                                  \
    {                                                                                   \
        strncpy_s(STR_.data, DBJ_COUNT_OF(STR_.data), S_, DBJ_COUNT_OF(STR_.data) - 1); \
    } while (0)
#else
#define DBJ_STRING_ASSIGN(STR_, S_)                          \
    do                                                       \
    {                                                        \
        strncpy(STR_.data, S_, DBJ_COUNT_OF(STR_.data) - 1); \
    } while (0)
#endif

// this is important
// variable can be in 3 states: exist, does not exist and exist but **empty**
// that is dbj::valstat related and is not that usual in programing languages
// but in e.g. databases it is a norm; field is always there but can be empty
// we declare empty dbj component string the one that has '\0' as the first char

#define DBJ_STRING_SET_EMPTY(STR_) (STR_.data[0] = '\0')

#define DBJ_STRING_IS_EMPTY(STR_) (STR_.data[0] == '\0')

DBJ_EXTERN_C_END

#endif // DBJ_STRING_INCLUDED