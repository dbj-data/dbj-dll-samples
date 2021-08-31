#include "dbj-shmem.h"
#include "../dbj-component.h"
#include "../dbj-string.h"

#define STB_DS_IMPLEMENTATION
#include <stb/stb_ds.h> // hash table

#include "shmemory.h"

DBJ_COMPONENT_VERSION_IMPLEMENTATION(0, 2, 0, "dbj shmem component");
/* --------------------------------------------------------------------------------- */
// dbj_component_can_unload_now() is part of dbj-component definition
// it is also generated here
DBJ_COMPONENT_UNLOADER_IMPLEMENTATION;
/* --------------------------------------------------------------------------------- */
/* here is the hash table to hold descriptors                                        */
static struct dbj_shmem_descriptor *shmem_descriptors_hash = 0;

__attribute__((destructor)) static void shmem_descriptors_hash_destructor(void)
{
    stbds_hmfree(shmem_descriptors_hash);
}

static void add_to_hash(struct dbj_shmem_descriptor shmem_descriptor_)
{
    stbds_hmputs(shmem_descriptors_hash, shmem_descriptor_);
}

// return the found one or the default return
// hmdefaults - Set the default struct used by hmget
// where ? what is the default return if we do not use that?
static struct dbj_shmem_descriptor find_in_hash(dbj_shmem_key_type key_)
{
    return stbds_hmgets(shmem_descriptors_hash, key_);
}

// true if to be deleted was found
// false otherwise
static bool remove_from_hash(dbj_shmem_key_type key_)
{
    // stbdes_hmdel returns 0 or 1
    return stbds_hmdel(shmem_descriptors_hash, key_);
}
/* --------------------------------------------------------------------------------- */
// static struct dbj_shmem_descriptor *shmem_descriptor_ = 0;
// static struct dbj_shmem_descriptor shmem_descriptor_instance_;
/* --------------------------------------------------------------------------------- */
/* 
will return the cached one if already created by this key
it seems  the WIN32 works unperturbed if shmem block by given name exists
will have to check on that carefully!
 */
static bool
create_method(struct component_shmem *self_, dbj_shmem_key_type key_, unsigned size_)
{
    struct dbj_shmem_descriptor cached_ = find_in_hash(key_);
    // default return in case not found is "all bits 0"
    if (cached_.block_size == 0)
    {
        struct dbj_shmem_descriptor shmem_descriptor_instance_ = dbj_shmem_descriptor_new(key_.data, size_);
        errno_t rez = dbj_shmem_create(&shmem_descriptor_instance_);
        if (0 == rez)
        {
            // ignore the key returned
            (void)add_to_hash(shmem_descriptor_instance_);
            return true;
        }
        errno = rez;
        return false;
    }
    errno = 0;
    return true;
}
static bool delete_method(struct component_shmem *self_, dbj_shmem_key_type key_)
{
    DBJ_ASSERT(self_);

    struct dbj_shmem_descriptor cached_ = find_in_hash(key_);
    // default return in case not found is "all bits 0"
    if (cached_.block_size == 0)
        return false;

    // remove from the shared memory
    errno_t erc = dbj_shmem_unmap(&cached_);
    if (0 != erc)
    {
        errno = erc;
        return false;
    }
    // remove from the HASH TABLE
    // theorethically might return false
    return remove_from_hash(key_);
}

static bool set_value_method(struct component_shmem *self_, dbj_shmem_key_type key_, unsigned size, void *value)
{
    struct dbj_shmem_descriptor cached_ = find_in_hash(key_);
    // default return in case not found is "all bits 0"
    if (cached_.block_size == 0)
        return false;

    DBJ_ASSERT(self_ && value);
    errno_t rez = dbj_shmem_set_shared_mem(&cached_, size, value);
    if (rez != 0)
    {
        errno = rez;
        return false;
    }
    return true;
}
static bool get_value_method(struct component_shmem *self_, dbj_shmem_key_type key_, unsigned size, void **value)
{
    struct dbj_shmem_descriptor cached_ = find_in_hash(key_);
    // default return in case not found is "all bits 0"
    if (cached_.block_size == 0)
        return false;

    DBJ_ASSERT(self_ && value);
    errno_t rez = dbj_shmem_get_shared_mem(&cached_, size, (unsigned char *)*value);
    if (0 != rez)
    {
        errno = rez;
        return false;
    }
    return true;
}
///////////////////////////////////////////////////////////////////////////////////
// the component implementation
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