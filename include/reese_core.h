#ifndef REESE_CORE
#define REESE_CORE

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#if defined _WIN32 || defined __CYGWIN__
#	ifdef REESE_EXPORTS
#		ifdef __GNUC__
#			define REESE_API __attribute__ ((dllexport))
#		else
#			define REESE_API __declspec(dllexport)
#		endif
#	else
#		ifdef __GNUC__
#			define REESE_API __attribute__ ((dllimport))
#		else
#			define REESE_API __declspec(dllimport)
#		endif
#	endif
#	define REESE_API_IGNORE
#else
#	if __GNUC__ >= 4
#           define REESE_API __attribute__ ((visibility ("default")))
#           define REESE_API_IGNORE  __attribute__ ((visibility ("hidden")))
#	else
#		define REESE_API
#		define REESE_API_IGNORE
#	endif
#endif

// Capture the instance and dereference it
#define capture(type, inst) (*(type*)reese_set_capture(&(inst)))
// Capture the instance without dereferencing (i.e., captures the instance and returns the pointer to it)
#define capture_ref(type, inst) ((type*)reese_set_capture(&(inst)))

// Finishes capturing the current instance
#define finish_capture reese_finish_capture

// Declares a member function in the class
#define REESE_DECLARE_MEMBER_FUNC(name, params) reese_end_capture_ret_type (*name) params;
// Forward declares a member function outside the class
#define REESE_FORWARD_DECLARE_MEMBER_FUNC(structure, name, params) reese_end_capture_ret_type structure##_##name##_##impl params;
// Defines a member function
#define REESE_DEFINE_MEMBER_FUNC(structure, name, params, body) reese_end_capture_ret_type structure##_##name##_##impl params { \
                                                                    structure *self = reese_get_capture();                      \
                                                                    reese_set_capture_result(NULL);                             \
                                                                    do {                                                        \
                                                                        body                                                    \
                                                                    } while(0);                                                 \
                                                                    reese_end_capture_ret_type ec;                              \
                                                                    ec.ret = &reese_get_capture_result;                         \
                                                                    return ec;                                                  \
                                                                }
// Binds a member function to the class
#define REESE_BIND_MEMBER_FUNC(structure, name) (self->name = &structure##_##name##_##impl)
// Returns the specified value through the member function
#define REESE_MEMBER_FUNC_RETURN(value) reese_set_capture_result(reese_allocate(sizeof (value), value)); break
// Returns null through the member function
#define REESE_MEMBER_FUNC_RETURN_NULL() reese_set_capture_result(NULL); break

// Forward declares the constructor outside the class
#define REESE_FORWARD_DECLARE_CONSTRUCTOR(structure, params) structure create_##structure params;
// Defines the constructor for the class
#define REESE_DEFINE_CONSTRUCTOR(structure, params, body) structure create_##structure params { \
                                                              structure __self;                 \
                                                              structure *self = &__self;        \
                                                              do {                              \
                                                                  body                          \
                                                              } while(0);                       \
                                                              return __self;                    \
                                                          }

// Fetches the constructor for the specified structure
#define REESE_FETCH_CONSTRUCTOR(structure) create_##structure
// Constructs an instance for the specified structure using the specified constructor arguments
#define REESE_CONSTRUCT(structure, ...) REESE_FETCH_CONSTRUCTOR(structure) (__VA_ARGS__)

// Casts the return value returned by a member function
#define ret_cast(type) *(const type*)

// This structure is provides the 'ret()' suffix to fetch the return value of a member function.
typedef struct {
    const void *(*ret)(void);
} reese_end_capture_ret_type;

// Dynamically allocates memory for storing a value
REESE_API void *reese_allocate(size_t byte_size, ...);

// Sets the variable for capture
REESE_API void *reese_set_capture(void *capture_var);
// Returns the variable currently being captured
REESE_API void *reese_get_capture(void);

// Sets the capture result (i.e., sets the pointer to the return value of the invoked member function)
REESE_API void reese_set_capture_result(void* capture_result);
// Returns the capture result (i.e., returns the pointer to the return value of the invoked member function)
REESE_API const void* reese_get_capture_result(void);

// Finishes capturing the current instance and frees the required memory
REESE_API void reese_finish_capture(void);

#ifdef __cplusplus
}
#endif

#endif /* REESE_CORE */
