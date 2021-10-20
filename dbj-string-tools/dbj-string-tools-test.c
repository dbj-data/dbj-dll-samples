// (c)  2021 by dbj at dbj dot org
#include <Windows.h>

// component header
#include "dbj-string-tools.h"

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#pragma region infrastructure
#ifdef _DEBUG
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
EXPORTS
dbj_component_can_unload_now    PRIVATE
dbj_component_factory           PRIVATE
dbj_component_version           PRIVATE
*/
#define DBJCS_CAN_UNLOAD_NAME "dbj_component_can_unload_now"
#define DBJCS_FACTORYNAME "dbj_component_factory"
#define DBJCS_SEMVER_NAME "dbj_component_version"

struct dbj_component_version_
{
    unsigned major;
    unsigned minor;
    unsigned patch;
    char description[0xFF];
};

typedef struct dbj_component_version_ (*DBJ_COMPONENT_SEMVER_FP)(void);
// unloading is also exactly the same for every DBJ Component
// because each one of them implements this function
typedef bool (*DBJ_COMPONENT_UNLOAD_FP)(void);

/*
return dll handle
*/
static inline HINSTANCE dbj_dll_load(
    /* just a file name, not a path! */
    const char dll_file_name_[static 1])
{
    HINSTANCE dll_handle_ = LoadLibraryExA(
        dll_file_name_,
        NULL,
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
#pragma endregion // infrastructure
/* ----------------------------------------------------------------------------------------------- */
static inline void test_driver(
    struct dbj_string_tools *dbj_st,
    const unsigned token_len, const char token[static token_len],
    const unsigned input_len, const char input[static input_len])
{
    unsigned result[input_len / 2 /*- token_len*/];

    DBG_PRINT("\n\nInput: \"%s\"\ntokens: \"%s\"", input, token);

    // Remember the result is, for example if 3 words found
    //  { words_found, w1b, w1e, w2b, w2e, w3b, w3e , 0, ... }
    //      0          1   2     3   4    5    6
    //
    const unsigned error_ = dbj_st->tokenizer(
        token_len, token,
        input_len, input,
        sizeof result, result);

    const unsigned no_of_words = result[0];

    DBG_PRINT("\nRezult\terrno:%3d , message: \"%s\"", error_, (error_ ? strerror(error_) : "OK"));

    if (error_ > 0)
        return;

    //  print the output array of locations returned
    //  { words_found, w1b, w1e, w2b, w2e, w3b, w3e , 0, ... }
    DBG_PRINT("\t, Output: { %3d ", no_of_words);
    for (unsigned w = 0, j = 1; w < (no_of_words); ++w, j += 2)
    {
        DBG_PRINT(" %3d %3d ", result[j], result[j + 1]);
    }
    DBG_PRINT(" }\n");

    //  print all the words found
    //  { words_found, w1b, w1e, w2b, w2e, w3b, w3e , 0, ... }
    for (unsigned w = 0, j = 1; w < no_of_words; ++w, j += 2)
    {
        DBG_PRINT("Word:%3d {%3d,%3d}\t", (w + 1), result[j], result[j + 1]);
        for (unsigned k = result[j]; k < result[j + 1]; ++k)
            DBG_PRINT("%c", input[k]);
        DBG_PRINT("\n");
    }
}
// this is a callback
// typedef struct dbj_string_tools *(*dbj_string_utils_factory_fp)(void);
static inline void dbj_tokenizer_test(dbj_string_utils_factory_fp factory)
{
    struct dbj_string_tools *dbj_st = factory();
    const char tokens[] = " +-/=";
    {
        const char input[] = "alfa + beta - gama = delta / epsilon ;";
        test_driver(dbj_st, sizeof tokens, tokens, sizeof input, input);
    }
    {
        const char input[] = "";
        test_driver(dbj_st, sizeof tokens, tokens, sizeof input, input);
    }
    {
        const char input[] = " +-/=";
        test_driver(dbj_st, sizeof tokens, tokens, sizeof input, input);
    }
    {
        const char input[] = "alfa"; // shorter vs tokens
        test_driver(dbj_st, sizeof tokens, tokens, sizeof input, input);
    }
    {
        const char input[] = "alfaalfa";
        test_driver(dbj_st, sizeof tokens, tokens, sizeof input, input);
    }
}
/* ----------------------------------------------------------------------------------------------- */
int main(int argc, char **argv)
{
    HINSTANCE dll_handle_ = dbj_dll_load(DBJ_STRING_TOOLS_DLL_NAME);

    if (0 == dll_handle_)
        return EXIT_FAILURE;

    // using the version info is exactly the same for every DBJ Component
    DBJ_COMPONENT_SEMVER_FP get_version =
        (DBJ_COMPONENT_SEMVER_FP)dbj_dll_get_function(&dll_handle_, DBJCS_SEMVER_NAME);
    if (get_version)
    {
        struct dbj_component_version_ info_ = get_version();
        DBG_PRINT("\n" DBJ_STRING_TOOLS_DLL_NAME " version info");
        DBG_PRINT("\nMajor: %d, minor: %d, patch: %d", info_.major, info_.minor, info_.patch);
        DBG_PRINT("\nDescription: %s", info_.description);
    }

    // specific for this component
    dbj_string_utils_factory_fp factory_fun =
        (dbj_string_utils_factory_fp)dbj_dll_get_function(&dll_handle_, DBJCS_FACTORYNAME);
    if (factory_fun)
        dbj_tokenizer_test(factory_fun);

    // unloading is also exactly the same for every DBJ Component
    DBJ_COMPONENT_UNLOAD_FP can_unload =
        (DBJ_COMPONENT_UNLOAD_FP)dbj_dll_get_function(&dll_handle_, DBJCS_CAN_UNLOAD_NAME);
    if (can_unload)
    {
        if (can_unload())
            if (!FreeLibrary(dll_handle_))
            {
                DBG_PRINT("\ndbjdll_load FreeLibrary failed. The DLL name is: %s\n", DBJ_STRING_TOOLS_DLL_NAME);
                return EXIT_FAILURE;
            }
    }

    return EXIT_SUCCESS;
}