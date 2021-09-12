#ifndef DBJ_SHMEMORY_INC_
#define DBJ_SHMEMORY_INC_

#include <dbj_capi/ccommon.h>
#include <dbj_capi/cdebug.h>
#include <dbj_capi/dbj_windows_include.h>

#include <memory.h>

#include <dbj_capi/dbj-string.h>

DBJ_EXTERN_C_BEGIN

struct dbj_shmem_descriptor
{
    dbj_string_32 key;
    unsigned block_size;
    LPVOID pointer_to_shared_memory; // pointer to shared memory
    HANDLE handle_to_file_mapping;   // handle to file mapping
};

static inline struct dbj_shmem_descriptor dbj_shmem_descriptor_new(
    const char new_name[static 1],
    const unsigned new_size)
{
    struct dbj_shmem_descriptor rezult = {
        .key = {{0}},
        .block_size = new_size,
        .pointer_to_shared_memory = 0,
        .handle_to_file_mapping = 0};
    DBJ_STRING_ASSIGN(rezult.key, new_name);
    return rezult;
}

//////////////////////////////////////////////////////////////////////////////

static inline errno_t dbj_shmem_create(struct dbj_shmem_descriptor *descriptor_)
{
    DBJ_ASSERT(descriptor_);
    descriptor_->handle_to_file_mapping = CreateFileMappingA(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security attributes
        PAGE_READWRITE,          // read/write access
        0,                       // size: high 32-bits
        descriptor_->block_size, // size: low 32-bits
        descriptor_->key.data);  // name of map object
    if (descriptor_->handle_to_file_mapping == NULL)
        return EINVAL;

    // is this the first caller
    BOOL fInit = (GetLastError() != ERROR_ALREADY_EXISTS);

    // Get a pointer to the file-mapped shared memory
    descriptor_->pointer_to_shared_memory = MapViewOfFile(
        descriptor_->handle_to_file_mapping, // object to map view of
        FILE_MAP_WRITE,                      // read/write access
        0,                                   // high offset:  map from
        0,                                   // low offset:   beginning
        0);                                  // default: map entire file
    if (descriptor_->pointer_to_shared_memory == NULL)
        return ENOMEM;

    // Initialize memory if this is the first caller on this key
    if (fInit)
        memset(descriptor_->pointer_to_shared_memory, '\0', descriptor_->block_size);

    return (errno_t)0;
}

static inline errno_t dbj_shmem_unmap(struct dbj_shmem_descriptor *descriptor_)
{
    BOOL fIgnore = UnmapViewOfFile(descriptor_->pointer_to_shared_memory);
    // Close the process's handle to the file-mapping object
    fIgnore = CloseHandle(descriptor_->handle_to_file_mapping);
    return (errno_t)0;
}

// SetSharedMem sets the contents of the shared memory
static inline errno_t dbj_shmem_set_shared_mem(
    struct dbj_shmem_descriptor *descriptor_,
    const unsigned size_,
    unsigned char retbuf[static size_])
{
    DBJ_ASSERT(descriptor_);
    DBJ_ASSERT(size_ <= descriptor_->block_size);
    // Copy into shared memory
    errno_t rez = memcpy_s(descriptor_->pointer_to_shared_memory, descriptor_->block_size, retbuf, size_);
    if (rez != 0)
        return EINVAL;
    return (errno_t)0;
}

// gets the contents of the shared memory
static inline errno_t dbj_shmem_get_shared_mem(
    struct dbj_shmem_descriptor *descriptor_,
    const unsigned size_,
    unsigned char retbuf[static size_])
{
    DBJ_ASSERT(descriptor_);
    DBJ_ASSERT(size_ <= descriptor_->block_size);
    // Copy from shared memory into the caller's buffer
    errno_t rez = memcpy_s(retbuf, size_, descriptor_->pointer_to_shared_memory, size_ <= descriptor_->block_size);
    if (rez != 0)
        return EINVAL;
    return (errno_t)0;
}

DBJ_EXTERN_C_END

#endif // DBJ_SHMEMORY_INC_
