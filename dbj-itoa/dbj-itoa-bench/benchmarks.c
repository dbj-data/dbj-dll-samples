/*
so far dbj_itoa() comes out as a winner

NOTE on gcvt() from the man page:

This function is marked as LEGACY in POSIX.1-2001. POSIX.1-2008 removes the specification of gcvt(), 
recommending the use of sprintf(3) instead (though snprintf(3) may be preferable).

*/
#include "../dbj-itoa.h"
#include "../../dbj-light-loader.h"
#include <ubut/ubench.h>

#pragma region shared state

// 512 is huge
#undef BUFSIZ
// 0xF is quite enough for everybody
#define BUFSIZ 0xF

struct state_
{
    bool initialized;
    HINSTANCE dll_handle; // = dbj_dll_load(DBJ_STRING_TOOLS_DLL_NAME);
    dbj_itoa_ifp interface_factory;
    char buffy[BUFSIZ];
};

static struct state_ state = {false, 0, 0, {0}};

__attribute__((constructor)) static void construct_state(void)
{
    HINSTANCE dll_handle_ = dbj_dll_load(DBJ_STRING_TOOLS_DLL_NAME);
    if (!dll_handle_)
    {
        state.initialized = false;
        return;
    }
    DBJ_DLL_IFP(dbj_itoa_ifp, interface_factory_, dll_handle_);
    if (!interface_factory_)
    {
        state.initialized = false;
        return;
    }

    state.initialized = true;
    state.dll_handle = dll_handle_;
    state.interface_factory = interface_factory_;
}

__attribute__((destructor)) static void destruct_state(void)
{
    if (state.initialized == true)
    {
        /* int rez = */ dbj_light_unload_dll(state.dll_handle);

        state.initialized = false;
        state.dll_handle = 0;
        state.interface_factory = 0;
    }
}

#pragma endregion

UBENCH(dbj_number_to_string, decimal_from)
{
    state.interface_factory()->decimal_from(LONG_MAX, BUFSIZ, state.buffy);
}

UBENCH(dbj_number_to_string, dbj_itoa)
{
    state.interface_factory()->dbj_itoa(state.buffy, LONG_MAX);
}

UBENCH(dbj_number_to_string, sprintf_unsigned_integer)
{
    (void)sprintf(state.buffy, "%u", UINT_MAX);
}

UBENCH(dbj_number_to_string, sprintf_unsigned_long_long)
{
    (void)sprintf(state.buffy, "%llu", ULLONG_MAX);
}

#pragma region mysql implementation

typedef unsigned long ulong;
typedef unsigned char uchar;

#define NullS NULL

// DBJ: removed the upcase argument
// this is apparently from MySQL code base?
// https://stackoverflow.com/a/36391225/10870835
static char *
int2str(long int val, char *dst, int radix)
{
    char buffer[65] = {0};
    char *p = 0;
    long int new_val = 0;

    ulong uval = (ulong)val;

    if (radix < 0)
    {
        if (radix < -36 || radix > -2)
            return NullS;
        if (val < 0)
        {
            *dst++ = '-';
            /* Avoid integer overflow in (-val) for LLONG_MIN (BUG#31799). */
            uval = (ulong)0 - uval;
        }
        radix = -radix;
    }
    else if (radix > 36 || radix < 2)
        return NullS;

    /*
    The slightly contorted code which follows is due to the fact that
    few machines directly support unsigned long / and %.  Certainly
    the VAX C compiler generates a subroutine call.  In the interests
    of efficiency (hollow laugh) I let this happen for the first digit
    only; after that "val" will be in range so that signed integer
    division will do.  Sorry 'bout that.  CHECK THE CODE PRODUCED BY
    YOUR C COMPILER.  The first % and / should be unsigned, the second
    % and / signed, but C compilers tend to be extraordinarily
    sensitive to minor details of style.  This works on a VAX, that's
    all I claim for it.
  */
    p = &buffer[sizeof(buffer) - 1];
    *p = '\0';
    new_val = uval / (ulong)radix;
    *--p = (uchar)(uval - (ulong)new_val * (ulong)radix);
    val = new_val;
    while (val != 0)
    {
        ldiv_t res;
        res = ldiv(val, radix);
        *--p = res.rem;
        val = res.quot;
    }
    while ((*dst++ = *p++) != 0)
        ;
    return dst - 1;
}

UBENCH(dbj_number_to_string, my_sql_int_2_str)
{
    char *rez =
        int2str(ULONG_MAX, state.buffy, 10 /*radix*/);

    (void)rez;
}

#pragma endregion

#pragma region microsoft specific

// DBJ: outcome is this is the slowest one
// https://docs.microsoft.com/de-de/cpp/c-runtime-library/reference/ecvt-s?view=msvc-160
UBENCH(dbj_number_to_string, microsoft_gcvt_s)
{
    int decimal;
    int sign;
    int err = _ecvt_s(state.buffy, BUFSIZ, INT_MAX, BUFSIZ, &decimal, &sign);
    (void)err;
}

#pragma endregion