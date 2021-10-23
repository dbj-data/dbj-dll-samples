#ifndef DBJ_VECTOR_INC_
#define DBJ_VECTOR_INC_
/*
simple vector as a DBJ Component
*/

#include <dbj_capi/dbj-string.h>
#include <stdbool.h>

DBJ_EXTERN_C_BEGIN

//#define DBJ_VCTR_SUCCESS        0
//#define DBJ_VCTR_MEMORY_ERROR  -1
//#define DBJ_VCTR_EINVAL        -2
//#define DBJ_VCTR_INTERN_ERROR  -3
//#define DBJ_VCTR_GEN_ERROR     -4

#define DBJ_VCTR_IS_SUCCESS(arg) ((arg) >= 0)
#define DBJ_VCTR_IS_ERROR(arg) ((arg) < 0)

//  make sure build creates this dll for this component
#define COMPONENT_FILENAME_DBJ_VECTOR "dbj-vector.dll"

typedef int dbj_status;
typedef struct dbj_vector dbj_vector_t;

//
struct dbj_vector_
{
	dbj_vector_t *(*create)(size_t /*item_size*/, size_t /*capacity*/);

	void (*destroy)(dbj_vector_t *);

	size_t (*size)(const dbj_vector_t *);

	size_t (*capacity)(const dbj_vector_t *);

	int (*push)(dbj_vector_t *, const void * /*item*/);

	int (*insert)(dbj_vector_t *, const void * /*item*/, size_t /*pos*/);

	dbj_vector_t *(*split)(dbj_vector_t *, size_t /*pos*/);

	const void *(*at_c)(const dbj_vector_t * /*vector*/, size_t /*index*/);

	void *(*at)(dbj_vector_t *, size_t /*index*/);

	dbj_status (*swap)(dbj_vector_t * /*cur_vec*/, dbj_vector_t * /*mv_vec*/, size_t /*pos*/);

	void (*clear)(dbj_vector_t *);

	int (*erase)(dbj_vector_t *, size_t /*index*/);
};

//
// FP of the interface_factory
// for this component
typedef struct dbj_vector_ *(*dbj_vector_ifp)(void);

DBJ_EXTERN_C_END

#endif // DBJ_VECTOR_INC_