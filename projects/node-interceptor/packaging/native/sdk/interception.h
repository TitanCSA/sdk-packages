#pragma once
// Compatibility ABI implemented by this project; no original Interception DLL.
#include <stddef.h>
#ifdef INTERCEPTION_STATIC
#define INTERCEPTION_API
#elif defined(INTERCEPTION_EXPORT)
#define INTERCEPTION_API __declspec(dllexport)
#else
#define INTERCEPTION_API __declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
enum { INTERCEPTION_MAX_KEYBOARD=10, INTERCEPTION_MAX_MOUSE=10, INTERCEPTION_MAX_DEVICE=20 };
typedef void* InterceptionContext;
typedef int InterceptionDevice;
typedef int InterceptionPrecedence;
typedef unsigned short InterceptionFilter;
typedef int (*InterceptionPredicate)(int);
typedef struct { unsigned short code,state; unsigned int information; } InterceptionKeyStroke;
typedef struct { unsigned short state,flags; short rolling; int x,y; unsigned int information; } InterceptionMouseStroke;
typedef char InterceptionStroke[sizeof(InterceptionMouseStroke)];
enum { INTERCEPTION_FILTER_KEY_NONE=0, INTERCEPTION_FILTER_KEY_ALL=65535,
       INTERCEPTION_FILTER_MOUSE_NONE=0, INTERCEPTION_FILTER_MOUSE_ALL=65535 };
INTERCEPTION_API InterceptionContext interception_create_context(void);
INTERCEPTION_API void interception_destroy_context(InterceptionContext);
INTERCEPTION_API int interception_get_precedence(InterceptionContext,int);
INTERCEPTION_API void interception_set_precedence(InterceptionContext,int,int);
INTERCEPTION_API unsigned short interception_get_filter(InterceptionContext,int);
INTERCEPTION_API void interception_set_filter(InterceptionContext,InterceptionPredicate,unsigned short);
INTERCEPTION_API int interception_wait(InterceptionContext);
INTERCEPTION_API int interception_wait_with_timeout(InterceptionContext,unsigned long);
INTERCEPTION_API int interception_receive(InterceptionContext,int,InterceptionStroke*,unsigned int);
INTERCEPTION_API int interception_send(InterceptionContext,int,const InterceptionStroke*,unsigned int);
INTERCEPTION_API unsigned int interception_get_hardware_id(InterceptionContext,int,void*,unsigned int);
INTERCEPTION_API int interception_is_keyboard(int);
INTERCEPTION_API int interception_is_mouse(int);
INTERCEPTION_API int interception_is_invalid(int);
// Extensions: stable errors, cancellation and read-only installation status.
INTERCEPTION_API unsigned long interceptor_error(void);
INTERCEPTION_API const char* interceptor_error_message(void);
INTERCEPTION_API int interceptor_installed(void);
INTERCEPTION_API void interceptor_cancel(InterceptionContext);
INTERCEPTION_API void interceptor_wake(InterceptionContext);
// Caller owns the duplicated Windows wait handle and must CloseHandle it.
INTERCEPTION_API void* interceptor_duplicate_ready_event(InterceptionContext);
#ifdef __cplusplus
}
#endif
