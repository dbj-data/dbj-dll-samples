#ifndef DBJ_COMPONENT_INCLUDED
#define DBJ_COMPONENT_INCLUDED
/*
one dbj component == one dll == one component interface == one struct

same def for all dll's:

EXPORTS
dbj_component_can_unload_now    PRIVATE
dbj_component_factory           PRIVATE
dbj_component_version           PRIVATE

*/

#include "dbj_capi/ccommon.h"
#include "dbj_capi/dbj_windows_include.h"

DBJ_EXTERN_C_BEGIN

// dbj_component_can_unload_now is part of each dbj-component definition
// use this macro to implement it inside a component dll
// dbj_component_can_unload_now() is required by dbj-component-loader
#define DBJ_COMPONENT_UNLOADER_IMPLEMENTATION                     \
    static volatile long component_counter_ = 0;                  \
                                                                  \
    __attribute__((constructor)) static void component_ctor(void) \
    {                                                             \
        _InterlockedIncrement(&component_counter_);               \
    }                                                             \
                                                                  \
    __attribute__((destructor)) static void component_dtor()      \
    {                                                             \
        _InterlockedDecrement(&component_counter_);               \
    }                                                             \
    bool dbj_component_can_unload_now(void)                       \
    {                                                             \
        return component_counter_ == 0;                           \
    }

/*
dbj-component working or not in the presence of MT is left to the requirements, ie. it is not
controlled by dbj-component design

The following actually might be NOT slower than using some global CRITICAL_SECTION
Usage : 

void fun ( void)
{
    dbj_component_lock(true)  ;
    //
    // do something
    //
    dbj_component_lock(false) ;
}
 */
#define DBJ_COMPONENT_LOCK_UNLOCK_FUNCTION    \
    static void dbj_component_lock(bool lock) \
    {                                         \
        static CRITICAL_SECTION CS_;          \
        if (lock)                             \
        {                                     \
            InitializeCriticalSection(&CS_);  \
            EnterCriticalSection(&CS_);       \
        }                                     \
        else                                  \
        {                                     \
            LeaveCriticalSection(&CS_);       \
            DeleteCriticalSection(&CS_);      \
        }                                     \
    }

#pragma region component semver

// each dbj-component has to exhibit semantic versioning 3 values
// and description string up to 0xFF chars long
// these structure is required for client code
struct dbj_component_version_
{
    unsigned major;
    unsigned minor;
    unsigned patch;
    char description[0xFF];
} ;

// use this macro to implement versioning inside a component
#define DBJ_COMPONENT_VERSION_IMPLEMENTATION(M, N, P, S_)              \
    struct dbj_component_version_ dbj_component_version(void)              \
    {                                                                  \
        static bool done_ = false;                                     \
        static struct dbj_component_version_ the_version_ = {M, N, P, {0}};      \
        if (!done_)                                                    \
        {                                                              \
            strncpy_s(the_version_.description, 0xFF, S_, strlen(S_)); \
            done_ = true;                                              \
        }                                                              \
        return the_version_;                                           \
    }

#pragma endregion // component semver
/*
one dbj component == one dll == one component interface == one struct

same def for all component dll's:

EXPORTS
dbj_component_can_unload_now    PRIVATE
dbj_component_factory           PRIVATE
dbj_component_version           PRIVATE

each is exported in a def file for each component
and is always the same name
regardless of the return type or arguments

*/
#define DBJ_COMPONENT_UNLOAD_NAME "dbj_component_can_unload_now"
#define DBJ_COMPONENT_FACTORYNAME "dbj_component_factory"
#define DBJ_COMPONENT_SEMVER_NAME "dbj_component_version"

/*
function footprints for these two are always the same
for them to be used client code need funcion pointer
declarations 

thus these two are always the same and declared in here
and nowehere else
*/
typedef struct dbj_component_version_ (*DBJ_COMPONENT_SEMVER_FP)(void);
typedef bool (*DBJ_COMPONENT_UNLOAD_FP)(void);

/*
factory function differs becuase it returns a pointer to the struct
that represents the component interface
We could made it return void *, but that would impose potentialy run-time consuming casting
*/

DBJ_EXTERN_C_END

#endif // DBJ_COMPONENT_INCLUDED