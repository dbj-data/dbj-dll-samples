#pragma once
#ifdef __clang__
#pragma clang system_header
#endif // __clang__
/*
(c)  2021 by dbj at dbj dot org, https://dbj.org/license_dbj

this might be the only dll load/unload infrastructure one might need
it all depend on if and what and when is OS caching 

*/
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#pragma region infrastructure

#ifdef _DEBUG
/*
this means we need to debug from Visual Studio or VS Code using inner terminal,
to actually see the debug output
which in turn means we do not need to build console testing apps in that case
*/
static inline void DBG_PRINT(const char *format_, ...)
{
    char buffy[1024] = {0};
    int nBuf = 0;
    va_list args = 0;
    va_start(args, format_);
    nBuf = _vsnprintf(buffy, 1024, format_, args);
    if (nBuf > -1)
        OutputDebugStringA(buffy);
    else
        OutputDebugStringA(__FILE__ " OutputDebugStringA buffer overflow\n");
    va_end(args);
}
#else
#define DBG_PRINT(format_, ...) ((void)0)
#endif

/*
These are well known and documented names, on the level of the DBJ CMS

EXPORTS
dbj_component_can_unload_now    PRIVATE
interface_factory           PRIVATE
dbj_component_version           PRIVATE
*/
#define DBJCS_CAN_UNLOAD_NAME "dbj_component_can_unload_now"
#define DBJCS_FACTORYNAME "interface_factory"
#define DBJCS_SEMVER_NAME "dbj_component_version"

struct dbj_component_version_
{
    unsigned major;
    unsigned minor;
    unsigned patch;
    char description[0xFF];
};

typedef struct dbj_component_version_ (*DBJ_COMPONENT_SEMVER_FP)(void);
/* 
unloading is also exactly the same for every DBJ Component aka DLL
because each one of them implements this function
But then, should we bother and unload manually at all? Or just leave it to the OS.
*/
typedef bool (*DBJ_COMPONENT_UNLOAD_FP)(void);

/*
return dll handle
one is hoping OS is caching the dll in memory for the next call
*/
static inline HINSTANCE dbj_dll_load(
    /* 
    just a file name, not a path! 
    */
    const char dll_file_name_[static 1])
{
    HINSTANCE dll_handle_ = LoadLibraryExA(
        dll_file_name_,
        NULL,
        /* we limit the locations of dll#s we load deliberately! */
        LOAD_LIBRARY_SEARCH_APPLICATION_DIR | LOAD_LIBRARY_SEARCH_SYSTEM32);

    if (NULL == dll_handle_)
    {
        DBG_PRINT("DLL Loader error: [%d]. "
                  "By design looking only in system folders and application folder."
                  " Could not find the DLL by name: %s",
                  GetLastError(), dll_file_name_);
    }
    return dll_handle_;
}

/*
one is hoping OS is caching the dll + function, in memory for the next call
*/
static inline void *dbj_dll_get_function(HINSTANCE *dll_handle_, char const fun_name_[static 1])
{
    // Funny fact: GetProcAddress has no unicode equivalent
    FARPROC result =
        GetProcAddress((HINSTANCE)*dll_handle_, fun_name_);
    if (result == 0)
    {
        DBG_PRINT("\nFailed to find the address for a function: %s\n", fun_name_);
    }
    return result;
}

static inline int dbj_light_report_version(HINSTANCE dll_handle_)
{
    // using the version info is exactly the same for every DBJ Component
    DBJ_COMPONENT_SEMVER_FP get_version =
        (DBJ_COMPONENT_SEMVER_FP)dbj_dll_get_function(&dll_handle_, DBJCS_SEMVER_NAME);

    if (!get_version)
    {
        DBG_PRINT("\nCould noty find function by name %s " DBJCS_SEMVER_NAME);
        return EINVAL;
    }
    struct dbj_component_version_ info_ = get_version();
    DBG_PRINT("\n" DBJ_STRING_TOOLS_DLL_NAME " version info");
    DBG_PRINT("\nMajor: %d, minor: %d, patch: %d", info_.major, info_.minor, info_.patch);
    DBG_PRINT("\nDescription: %s", info_.description);
    return 0;
}

static inline int dbj_light_unload_dll(HINSTANCE dll_handle_)
{
    // unloading is also exactly the same for every DBJ Component
    DBJ_COMPONENT_UNLOAD_FP can_unload =
        (DBJ_COMPONENT_UNLOAD_FP)dbj_dll_get_function(&dll_handle_, DBJCS_CAN_UNLOAD_NAME);
    if (!can_unload)
    {
        DBG_PRINT("\nCan not get the function by name : %s\n", DBJCS_CAN_UNLOAD_NAME);
        return EXIT_FAILURE;
    }

    if (can_unload())
        if (!FreeLibrary(dll_handle_))
        {
            DBG_PRINT("\ndbjdll_load FreeLibrary failed. The DLL name is: %s\n", DBJ_STRING_TOOLS_DLL_NAME);
            return EXIT_FAILURE;
        }
    return EXIT_SUCCESS;
}

#pragma endregion // infrastructure