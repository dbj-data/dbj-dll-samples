#include "component-b.h"
#include "../dbj-component.h"
#include "../dbj-string.h"
//
// #include <string.h>
// #include <intrin.h>

#include "shmemory.h"

DBJ_COMPONENT_VERSION_IMPLEMENTATION(0, 2, 0, "dbj shmem component");
/* --------------------------------------------------------------------------------- */
// dbj_component_can_unload_now() is part of dbj-component definition
// it is also generated here
DBJ_COMPONENT_UNLOADER_IMPLEMENTATION;
/* --------------------------------------------------------------------------------- */
/* here the hash table to hold descriptors, key is the name                          */
static struct dbj_shmem_descriptor *shmem_descriptor_ = 0;
static struct dbj_shmem_descriptor shmem_descriptor_instance_;
/* --------------------------------------------------------------------------------- */
static bool
create_method(struct component_shmem *self_, dbj_string_64 key_, unsigned size_)
{
    if (!shmem_descriptor_)
    {
        shmem_descriptor_instance_ = dbj_shmem_descriptor_new(key_.data, size_);
        shmem_descriptor_ = &shmem_descriptor_instance_;
        errno_t rez = dbj_shmem_create(shmem_descriptor_);
        if (0 == rez)
            return true;
        errno = rez;
        return false;
    }
    errno = 0;
    return true;
}
static bool delete_method(struct component_shmem *self_, dbj_string_64 key_)
{
    DBJ_ASSERT(shmem_descriptor_ && self_);
    if (!shmem_descriptor_)
        return false;
    errno_t erc = dbj_shmem_unmap(shmem_descriptor_);
    if (0 != erc)
    {
        errno = erc;
        return false;
    }
    return true;
}

static bool set_value_method(struct component_shmem *self_, dbj_string_64 key_, unsigned size, void *value)
{
    DBJ_GULP(key_);
    DBJ_ASSERT(shmem_descriptor_ && self_ && value);
    errno_t rez = dbj_shmem_set_shared_mem(shmem_descriptor_, size, value);
    if (rez != 0)
    {
        errno = rez;
        return false;
    }
    return true;
}
static bool get_value_method(struct component_shmem *self_, dbj_string_64 key_, unsigned size, void **value)
{
    DBJ_GULP(key_);
    DBJ_ASSERT(shmem_descriptor_ && self_ && value);
    errno_t rez = dbj_shmem_get_shared_mem(shmem_descriptor_, size, (unsigned char *)*value);
    if (0 != rez)
    {
        errno = rez;
        return false;
    }
    return true;
}
///////////////////////////////////////////////////////////////////////////////////
// private instance of the interface is the component implementation
static struct component_shmem
    componenet_implementation_ =
        {
            .create = create_method,
            .delete = delete_method,
            .get_value = get_value_method,
            .set_value = set_value_method};

// each dbj component must have exported function
// with the name "dbj_component_factory"
// see the def file in this folder
struct component_shmem *dbj_component_factory(void)
{
    return &componenet_implementation_;
}