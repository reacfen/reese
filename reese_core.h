#ifndef REESE_CORE
#define REESE_CORE

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define capture(type, var) (*(type*)reese_set_capture(&(var)))
#define capture_ptr(type, var) ((type*)reese_set_capture(&(var)))

#define finish_capture reese_finish_capture

#define REESE_DECLARE_MEMBER_FUNC(name, params) reese_end_capture_ret_type (*name) params;
#define REESE_FORWARD_DECLARE_MEMBER_FUNC(structure, name, params) reese_end_capture_ret_type structure##_##name##_##impl params;
#define REESE_DEFINE_MEMBER_FUNC(structure, name, params, body) reese_end_capture_ret_type structure##_##name##_##impl params { \
                                                                    structure *self = reese_get_capture();                      \
                                                                    reese_set_capture_result(NULL);                             \
                                                                    do body while(0);                                           \
                                                                    reese_end_capture_ret_type ec;                              \
                                                                    ec.ret = &reese_get_capture_result;                         \
                                                                    return ec;                                                  \
                                                                }
#define REESE_BIND_MEMBER_FUNC(structure, name) (self->name = &structure##_##name##_##impl)
#define REESE_MEMBER_FUNC_RETURN(value) reese_set_capture_result(reese_allocate(sizeof (value), value))
#define REESE_MEMBER_FUNC_RETURN_NULL() reese_set_capture_result(NULL)

#define REESE_FORWARD_DECLARE_CONSTRUCTOR(structure, params) structure create_##structure params;
#define REESE_DEFINE_CONSTRUCTOR(structure, params, body) structure create_##structure params { \
                                                              structure __self;                 \
                                                              structure *self = &__self;        \
                                                              do body while(0);                 \
                                                              return __self;                    \
                                                          }

#define REESE_FETCH_CONSTRUCTOR(structure) create_##structure

#define REESE_RET_CAST(type) *(const type*)

typedef struct {
    const void *(*ret)(void);
} reese_end_capture_ret_type;

void *reese_allocate(size_t byte_size, ...);

void *reese_set_capture(void *capture_var);
void *reese_get_capture(void);

void reese_set_capture_result(void* capture_result);
const void* reese_get_capture_result(void);

void reese_finish_capture(void);

#ifdef __cplusplus
}
#endif

#endif /* REESE_CORE */
