
#include "../dbj-itoa.h"
#include "../../dbj-light-loader.h"

#define DBJ_REVERSE_TEST
#include "../src/dbj_reverse.h" // dbj_reverse_test()

extern int test_decimal_from(dbj_itoa *);
int itoa_test(dbj_itoa *);

/* 
 most if not all errors are already reported; in _DEBUG builds
*/
int main(int argc, char **argv)
{
    // just a quick ad-hoc dbj reverse test
    dbj_reverse_test();

    HINSTANCE dll_handle_ = dbj_dll_load(DBJ_STRING_TOOLS_DLL_NAME);

    if (0 == dll_handle_)
        return EXIT_FAILURE;

    if (0 != dbj_light_version_report(dll_handle_))
        return EXIT_FAILURE;

    // dbj_itoa_ifp interface_factory =
    //     (dbj_itoa_ifp)dbj_dll_get_function(&dll_handle_, DBJCS_FACTORYNAME);

    DBJ_DLL_IFP(dbj_itoa_ifp, interface_factory, dll_handle_);

    if (!interface_factory)
    {
        return EXIT_FAILURE;
    }

    test_decimal_from(interface_factory());
    itoa_test(interface_factory());

    return dbj_light_unload_dll(dll_handle_);
}
