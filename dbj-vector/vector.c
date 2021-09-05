#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS /* To disable warnings for unsafe functions */
#endif

/*
    2021 SEP 04     DBJ     This is now "under" dbj_capi/ccommon.h
*/
#include "vector.h"
// #include <crtdbg.h>
// #include <string.h>
// #include <stdlib.h>

#undef MAX
#undef MIN
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

struct dbj_vector {
    size_t m_size;
    void  *m_data;
    size_t m_capacity;
    size_t m_item_size;
};


static int vector_reallocate_(dbj_vector_t *vector, size_t new_capacity)
{
    DBJ_ASSERT(vector);
    DBJ_ASSERT(new_capacity > vector->m_capacity);

    size_t new_size = new_capacity * vector->m_item_size;
    vector->m_data = realloc(vector->m_data, new_size);
    if (vector->m_data == NULL)
        return -1;
    return 0;
}


DBJ_VCTR_API
dbj_vector_t *create_vector(size_t item_size, size_t capacity)
{
   dbj_vector_t *vector = (dbj_vector_t *)malloc(sizeof(dbj_vector_t));
    if (vector == NULL) {
        return NULL;
    }

    size_t init_size = MAX(item_size * capacity, 1);
    vector->m_data = malloc(init_size);
    if (vector->m_data == NULL) {
        free(vector);
        return NULL;
    }

    vector->m_size      = 0;
    vector->m_capacity  = capacity;
    vector->m_item_size = item_size;

    return vector;
}


DBJ_VCTR_API
void destroy_vector(dbj_vector_t *vector)
{
    DBJ_ASSERT(vector);
    free(vector->m_data);
    free(vector);
}


DBJ_VCTR_API
size_t vector_size(const dbj_vector_t *vector)
{
    DBJ_ASSERT(vector);
    return vector->m_size;
}


DBJ_VCTR_API
size_t vector_capacity(const dbj_vector_t *vector)
{
    DBJ_ASSERT(vector);
    return vector->m_capacity;
}


DBJ_VCTR_API
int vector_push(dbj_vector_t *vector, const void *item)
{
    DBJ_ASSERT(vector);
    DBJ_ASSERT(item);

    if (vector->m_size == vector->m_capacity) {
        if (vector_reallocate_(vector, vector->m_capacity * 2) == -1)
            return DBJ_VCTR_GEN_ERROR;
        vector->m_capacity = vector->m_capacity * 2;
    }

    size_t offset = vector->m_size * vector->m_item_size;
    memcpy((char *)vector->m_data + offset, item, vector->m_item_size);

    ++(vector->m_size);

    return DBJ_VCTR_SUCCESS;
}

DBJ_VCTR_API
int vector_insert(dbj_vector_t *vector, const void *item, size_t pos)
{
    DBJ_ASSERT(vector);
    DBJ_ASSERT(item);
    size_t needed_capacity = MAX(pos + 1, vector->m_size + 1);
    if (vector->m_capacity < needed_capacity) {
        if (vector_reallocate_(vector, needed_capacity) == -1)
            return DBJ_VCTR_GEN_ERROR;
        vector->m_capacity = needed_capacity;
    }
    size_t offset = pos * vector->m_item_size;
    if (pos >= vector->m_size) {
        /* Data in the middle are not initialized */
        memcpy((char *)vector->m_data + offset, item, vector->m_item_size);
        vector->m_size = pos + 1;
        return DBJ_VCTR_SUCCESS;
    } else {
        /* Shift following data by one position */
        memmove((char *)vector->m_data + offset + vector->m_item_size,
                (char *)vector->m_data + offset,
                vector->m_item_size * (vector->m_size - pos));
        memcpy((char *)vector->m_data + offset, item, vector->m_item_size);
        ++(vector->m_size);
        return DBJ_VCTR_SUCCESS;
    }
}

DBJ_VCTR_API
dbj_vector_t *vector_split(dbj_vector_t *vector, size_t pos)
{
    size_t trailing_sz = vector->m_size > pos ? vector->m_size - pos : 0;
   dbj_vector_t *new_vector = create_vector(vector->m_item_size, trailing_sz);
    if (!new_vector)
        return new_vector;
    if (new_vector->m_capacity < trailing_sz) {
        destroy_vector(new_vector);
        return NULL;
    }

    if (trailing_sz == 0)
        return new_vector;

    size_t offset = vector->m_item_size * pos;
    memcpy(new_vector->m_data, (char *)vector->m_data + offset,
           trailing_sz * vector->m_item_size);
    new_vector->m_size = trailing_sz;
    vector->m_size = pos;
    return new_vector;
}

DBJ_VCTR_API
const void *vector_at_c(const dbj_vector_t *vector, size_t index)
{
    if (index >= vector->m_size)
        return NULL;

    return (char *)vector->m_data + index * vector->m_item_size;
}


DBJ_VCTR_API
void *vector_at(dbj_vector_t *vector, size_t index)
{
    if (index >= vector->m_size)
        return NULL;

    return (char *)vector->m_data + index * vector->m_item_size;
}


DBJ_VCTR_API
dbj_status vector_swap(dbj_vector_t *cur_vec,dbj_vector_t *mv_vec, size_t pos)
{
    DBJ_ASSERT(cur_vec);
    DBJ_ASSERT(mv_vec);
    DBJ_ASSERT(cur_vec != mv_vec);
    DBJ_ASSERT(cur_vec->m_item_size == mv_vec->m_item_size);

    size_t cur_sz = vector_size(cur_vec);
    size_t mv_sz = vector_size(mv_vec);
    if (mv_sz == 0) {
        return DBJ_VCTR_SUCCESS;
    }

    size_t min_targ_size = pos + mv_sz;
    if (vector_capacity(cur_vec) < min_targ_size) {
        if (vector_reallocate_(cur_vec, min_targ_size) == -1)
            return DBJ_VCTR_GEN_ERROR;
        cur_vec->m_capacity = min_targ_size;
    }

    size_t offset = pos * cur_vec->m_item_size;
    void *tmp = NULL;
    size_t new_mv_sz = 0;
    if (cur_sz > pos) {
        new_mv_sz = MIN(cur_sz - pos, mv_sz);
        tmp = malloc(cur_vec->m_item_size * new_mv_sz);
        if (tmp == NULL) {
            return DBJ_VCTR_MEMORY_ERROR;
        }
    }

    if (tmp) {
        memcpy(tmp,
               (char *)cur_vec->m_data + offset,
               cur_vec->m_item_size * new_mv_sz);
    }

    memcpy((char *)cur_vec->m_data + offset,
           mv_vec->m_data,
           cur_vec->m_item_size * mv_sz);

    if (tmp) {
        memcpy(mv_vec->m_data,
               tmp,
               cur_vec->m_item_size * new_mv_sz);
    }

    cur_vec->m_size = MAX(cur_vec->m_size, min_targ_size);
    mv_vec->m_size = new_mv_sz;
    free(tmp);
    return DBJ_VCTR_SUCCESS;
}

DBJ_VCTR_API
void vector_clear(dbj_vector_t *vector)
{
    vector->m_size = 0;
}

DBJ_VCTR_API
int vector_erase(dbj_vector_t *vector, size_t index)
{
    DBJ_ASSERT(vector);

    if (vector->m_size == 0 || index >= vector->m_size)
        return DBJ_VCTR_GEN_ERROR;

    memmove((char *)vector->m_data + vector->m_item_size * index,
            (char *)vector->m_data + vector->m_item_size * (index + 1),
            (vector->m_size - 1 - index) * vector->m_item_size);
    vector->m_size--;
    return DBJ_VCTR_SUCCESS;
}

#ifdef DBJ_VCTR_TEST_BUILD

dbj_vector_t *copy_vector(dbj_vector_t *v)
{
    if (v == NULL)
        return NULL;

   dbj_vector_t *new_vector = create_vector(v->m_item_size, v->m_capacity);
    if (new_vector == NULL)
        return NULL;

    memcpy(new_vector->m_data, v->m_data, v->m_item_size * v->m_size);
    new_vector->m_size      = v->m_size ;
    new_vector->m_item_size = v->m_item_size ;
    return new_vector;
}

size_t vector_index_of(const dbj_vector_t *vector, const void *item)
{
    DBJ_ASSERT(vector);
    DBJ_ASSERT(item);

    size_t i = 0;
    for (i = 0; i < vector->m_size; ++i) {
        void *data_pos = (char *)vector->m_data + i * vector->m_item_size;
        if (memcmp(data_pos, item, vector->m_item_size) == 0) {
            return i;
        }
    }
    return INVALID_VEC_INDEX;
}

#endif