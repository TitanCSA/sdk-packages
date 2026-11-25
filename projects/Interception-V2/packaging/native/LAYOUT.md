# Native SDK

This directory contains the shared native SDK source and the CMake, Conan, vcpkg and NuGet metadata. Registry-specific metadata with differing contents is preserved under `registry-metadata/`.

Build with `cmake -S packaging/native -B build/native -A x64`, then `cmake --build build/native --config Release`. No kernel driver is installed.

For a native Visual Studio project, install `Interception-V2.Native` from NuGet.org. Its targets compile the original C implementation into your app. If you need the original `interception.dll`, use the matching SDK from the product demo ZIP.

Use the [NuGet manifest](../../../../nuget-manifest.json) for the reviewed package versions. The 2.0.1 packaging update allows builds with `/WX` by applying the legacy MSVC CRT warning exception to `interception.c` alone. The SDK source, headers and driver version stay at 2.0.0.
