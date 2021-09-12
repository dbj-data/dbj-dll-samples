#include "dbj-syserrmsg.h"
#include "../dbj-component.h"

DBJ_COMPONENT_VERSION_IMPLEMENTATION(0, 1, 0, "dbj syserrmsg component");
/* --------------------------------------------------------------------------------- */
DBJ_COMPONENT_UNLOADER_IMPLEMENTATION;
/* --------------------------------------------------------------------------------- */
static dbj_string_512 error_message_implementation_(struct component_syserrmsg *self_, const unsigned error_code)
{
    dbj_string_512 msg_ = {{""}};
    return msg_;
}
/* --------------------------------------------------------------------------------- */
/* interface implementation */
static struct component_syserrmsg interface_implementation_ = {.error_message = error_message_implementation_};

struct component_syserrmsg *dbj_component_factory(void)
{
    return &interface_implementation_;
}