// Bind to the running Node/Electron host even when its EXE has another name.
#include <windows.h>
#include <delayimp.h>
#include <cstring>
namespace {
FARPROC WINAPI HostModule(unsigned event,DelayLoadInfo* info) {
    if(event!=dliNotePreLoadLibrary || _stricmp(info->szDll,"node.exe"))return nullptr;
    const auto dll=GetModuleHandleW(L"libnode.dll");
    return reinterpret_cast<FARPROC>(dll?dll:GetModuleHandleW(nullptr));
}
}
decltype(__pfnDliNotifyHook2) __pfnDliNotifyHook2=HostModule;
