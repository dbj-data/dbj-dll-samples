// (c)  2021 by dbj at dbj dot org

// component header
#include "dbj-string-tools.h"

#include "../dbj-light-loader.h"

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

static inline void dbj_tokenizer_test(struct dbj_string_tools *dbj_st)
{

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
/* 
 most if not all error are already reported 
*/
int main(int argc, char **argv)
{
    HINSTANCE dll_handle_ = dbj_dll_load(DBJ_STRING_TOOLS_DLL_NAME);

    if (0 == dll_handle_)
        return EXIT_FAILURE;

    if (0 != dbj_light_report_version(dll_handle_))
        return EXIT_FAILURE;

    // specific factory for this component only
    // note: factory function name is always the same
    dbj_string_utils_factory_fp factory_fun =
        (dbj_string_utils_factory_fp)dbj_dll_get_function(&dll_handle_, DBJCS_FACTORYNAME);

    if (!factory_fun)
    {
        return EXIT_FAILURE;
    }

    dbj_tokenizer_test(factory_fun());

    return dbj_light_unload_dll(dll_handle_);
}
