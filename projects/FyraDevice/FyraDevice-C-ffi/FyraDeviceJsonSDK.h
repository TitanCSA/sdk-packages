/* SPDX-License-Identifier: MIT */
#ifndef FYRADEVICE_CSDK_H
#define FYRADEVICE_CSDK_H
#include <stdint.h>
#if !defined(_WIN32) || !defined(_WIN64)
#error This SDK requires Windows x64.
#endif
#ifdef __cplusplus
#define FYRADEVICE_CSDK_NOEXCEPT noexcept
extern "C" {
#else
#define FYRADEVICE_CSDK_NOEXCEPT
#endif
#if defined(FYRADEVICE_CSDK_BUILD)
#define FYRADEVICE_CSDK_API __declspec(dllexport)
#else
#define FYRADEVICE_CSDK_API __declspec(dllimport)
#endif
/* open is inert. One active session per loaded product DLL.
   call accepts length-delimited UTF-8 JSON and returns an allocated UTF-8
   response, or NULL on allocation failure. Always free with this same DLL.
   Session/operation handles are scoped to the client that returned them. */
FYRADEVICE_CSDK_API uint32_t fyradevice_json_open(uint64_t *session) FYRADEVICE_CSDK_NOEXCEPT;
FYRADEVICE_CSDK_API uint32_t fyradevice_json_close(uint64_t session) FYRADEVICE_CSDK_NOEXCEPT;
FYRADEVICE_CSDK_API char *fyradevice_json_call(uint64_t session,const char *request,uint32_t length) FYRADEVICE_CSDK_NOEXCEPT;
FYRADEVICE_CSDK_API void fyradevice_json_free(char *response) FYRADEVICE_CSDK_NOEXCEPT;
#ifdef __cplusplus
}
#endif
#undef FYRADEVICE_CSDK_NOEXCEPT
#endif
