#ifndef DBJ_VCTR_H
#define DBJ_VCTR_H

#include <dbj_capi/ccommon.h>

DBJ_EXTERN_C_BEGIN

/*
 *               RETURN CODES
 */

#define DBJ_VCTR_SUCCESS        0
#define DBJ_VCTR_MEMORY_ERROR  -1
#define DBJ_VCTR_EINVAL        -2
#define DBJ_VCTR_INTERN_ERROR  -3
#define DBJ_VCTR_GEN_ERROR     -4
#define DBJ_VCTR_IS_SUCCESS(arg) ((arg) >= 0)
#define DBJ_VCTR_IS_ERROR(arg) ((arg) < 0)

/*
 *
 */

#define DBJ_VCTR_API 

typedef int dbj_status;
typedef struct dbj_vector dbj_vector_t;


#define INVALID_VEC_INDEX ((size_t) -1)

DBJ_VCTR_API dbj_vector_t *create_vector(size_t item_size, size_t capacity);

DBJ_VCTR_API void destroy_vector(dbj_vector_t *);

DBJ_VCTR_API size_t vector_size(const dbj_vector_t *);

DBJ_VCTR_API size_t vector_capacity(const dbj_vector_t *);

DBJ_VCTR_API int vector_push(dbj_vector_t *, const void *item);

DBJ_VCTR_API int vector_insert(dbj_vector_t *, const void *item, size_t pos);

DBJ_VCTR_API dbj_vector_t *vector_split(dbj_vector_t *, size_t pos);

DBJ_VCTR_API const void *vector_at_c(const dbj_vector_t *vector, size_t index);

DBJ_VCTR_API void *vector_at(dbj_vector_t *, size_t index);

DBJ_VCTR_API dbj_status vector_swap(dbj_vector_t *cur_vec,dbj_vector_t *mv_vec, size_t pos);

DBJ_VCTR_API void vector_clear(dbj_vector_t *);

DBJ_VCTR_API int vector_erase(dbj_vector_t *, size_t index);

#ifdef DBJ_VCTR_TEST_BUILD
DBJ_VCTR_API dbj_vector_t *copy_vector(dbj_vector_t *);
DBJ_VCTR_API size_t vector_index_of(const dbj_vector_t *, const void *item);
#endif

#define DBJ_VCTR_AT(vector, pos, data_type) \
    *(data_type *)vector_at((vector), (pos))

#define DBJ_VCTR_AT_C(vector, pos, const_data_type) \
    *(const_data_type *)vector_at_c((vector), (pos))

DBJ_EXTERN_C_END

#endif /* DBJ_VCTR_H */