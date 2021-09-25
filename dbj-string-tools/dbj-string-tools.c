#include "dbj-string-tools.h"

#include "../dbj-component.h"
// #include <dbj_capi/dbj-string.h>

// #include <intrin.h>

// dbj_component_version() generated here
DBJ_COMPONENT_VERSION_IMPLEMENTATION(0, 1, 0, "dbj string operations");
/* --------------------------------------------------------------------------------- */
// dbj_component_can_unload_now() is part of dbj-component definition
// it is also generated here
DBJ_COMPONENT_UNLOADER_IMPLEMENTATION;
/* --------------------------------------------------------------------------------- 
(c) 2021 by dbj@dbj.org https://license_dbj
this tokenzier was completely dreamt-up by me
*/
static inline bool char_in_string(const char chr_, unsigned len_, const char str_[static len_])
{
    for (unsigned j = 0; j < len_; ++j)
    {
        if (str_[j] == chr_)
            return true;
    }
    return false;
}

static inline unsigned tokenzier_back(
    unsigned output_idx, // the last end pos recorded or 0
    char *walker_,
    const unsigned token_len, const char token[static token_len],
    const unsigned input_len, const char input[static input_len],
    const unsigned output_len, unsigned output[static output_len])
{
    // skip the token prefix (if any) for the first word start pos
    while (char_in_string(*walker_, token_len, token))
    {
        ++walker_;
    }

    output_idx += 1;
    output[output_idx] = (unsigned)(walker_ - input); // record the word start pos

    // skip to the token (if any) for the next iteration
    while (!char_in_string(*walker_, token_len, token))
    {
        ++walker_;
    }

    output_idx += 1;
    output[output_idx] = (unsigned)(walker_ - input); // record the word end pos

    if (output[output_idx] >= input_len)
    {
        // save the num of words into the position 0
        output[0] = output_idx / 2;
        return output[0];
    }
    else
        return tokenzier_back(output_idx, walker_, token_len, token, input_len, input, output_len, output);
}

static inline unsigned tokenzier(
    const unsigned token_len, const char token[static token_len],
    const unsigned input_len, const char input[static input_len],
    const unsigned output_len, unsigned output[static output_len])
{
    errno = 0; // important!

    if ((token_len < 1) || (input_len < token_len) || (input_len >= dbj_string_tools_max_input_count))
    {
        errno = EINVAL;
        return 0;
    }

    // empty input
    if (input[0] == '\0')
    {
        errno = EINVAL;
        return 0;
    }

    char *walker_ = (char *)input;

    // solve the pathological case when there is only tokens in the input
    // or input is empty
    while (char_in_string(*walker_, token_len, token))
    {
        if (*walker_ == '\0')
        {
            errno = EINVAL;
            return 0;
        }
        ++walker_;
    }

    return tokenzier_back(0, walker_,
                          token_len, token,
                          // this -1 is crucial !
                          input_len - 1, input,
                          output_len, output);
}
/* --------------------------------------------------------------------------------- */
//  private instance of the interface implementation
/* --------------------------------------------------------------------------------- */
static struct dbj_string_tools component_implementation_ = {
    .tokenizer = tokenzier};

// each dbj component must have exported factory function
// with the name "dbj_component_factory"
// see the def file in this folder
struct dbj_string_tools *dbj_component_factory(void)
{
    return &component_implementation_;
}