
#include "../dbj-itoa.h"
#include "../../dbj-light-loader.h"

extern int test_decimal_from(dbj_itoa *);
int itoa_test(dbj_itoa *);

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
    // return type is always different
    dbj_itoa_factory_fp factory_fun =
        (dbj_itoa_factory_fp)dbj_dll_get_function(&dll_handle_, DBJCS_FACTORYNAME);

    if (!factory_fun)
    {
        return EXIT_FAILURE;
    }

    test_decimal_from(factory_fun());
    itoa_test(factory_fun());

    return dbj_light_unload_dll(dll_handle_);
}
