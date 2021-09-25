#define DBJ_CAPI_DEFAULT_LOG_IMPLEMENTATION
#define DBJCS_DLL_CALLER_IMPLEMENTATION
#define STB_DS_IMPLEMENTATION

#include <dbj_capi/cdebug.h>
#include "../dbj-component.h"
#include <dbj_capi/dbj-string.h>
#include "../dbj-component-loader.h"

// components are partitioned in their own projects
#include "dbj-string-tools.h"

#include <assert.h>
#include <stdio.h>
/* ----------------------------------------------------------------------------------------------- */
// same for each component
static inline void get_version_cb(DBJ_COMPONENT_SEMVER_FP get_version)
{
    struct dbj_component_version_ info_ = get_version();
    DBG_PRINT("\ncomponent dll version info");
    DBG_PRINT("\nMajor: %d, minor: %d, patch: %d", info_.major, info_.minor, info_.patch);
    DBG_PRINT("\nDescription: %s", info_.description);
}
/* ----------------------------------------------------------------------------------------------- */
static void show_component_info(const char component_dll_name[static 1])
{
    DBJCS_ANY_CALL(component_dll_name,
                   /* same for each component */
                   DBJCS_SEMVER_NAME,
                   /* same for each component */
                   DBJ_COMPONENT_SEMVER_FP,
                   /* local call back */
                   get_version_cb);
}

/* ----------------------------------------------------------------------------------------------- */
static inline void test_driver(
    struct dbj_string_tools *dbj_st,
    const unsigned token_len, const char token[static token_len],
    const unsigned input_len, const char input[static input_len])
{
    unsigned result[input_len / 2 /*- token_len*/];

    DBG_PRINT("\n\nInput: \"%s\"\ntokens: \"%s\"", input, token);

    const unsigned error_ = dbj_st->tokenizer(
        token_len, token,
        input_len, input,
        sizeof result, result);

    const unsigned no_of_words = result[0];

    DBG_PRINT("\nRezult\terrno:%3d , message: \"%s\"", error_, (error_ ? strerror(error_) : "OK"));

    if (error_ > 0)
        return;

    DBG_PRINT("\t, Output: { %3d ", no_of_words);
    for (unsigned w = 0, j = 1; w < (no_of_words); ++w, j += 2)
    {
        DBG_PRINT(" %3d %3d ", result[j], result[j + 1]);
    }
    DBG_PRINT(" }\n");

    //  this is somewhat peculiar , remember result is
    // for example if 3 words found
    //  { words_num, w1b, w1e, w2b, w2e, w3b, w3e , 0, ... }
    //      0          1   2     3   4    5    6
    //
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
    DBJCS_LOADER_LOG("Starting: %s", argv[0]);

    show_component_info(DBJ_STRING_TOOLS_DLL_NAME);
    DBJCS_CALL(DBJ_STRING_TOOLS_DLL_NAME, dbj_string_utils_factory_fp, dbj_tokenizer_test);

    DBJCS_LOADER_LOG("Ending: %s", argv[0]);
    return 0;
}