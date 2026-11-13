# AutoHotInterceptor .NET SDK

The existing AutoHotInterception managed SDK for .NET Framework 4.8, with its matching
Windows x86/x64 native bridge. Namespace: `AutoHotInterception`. The public entry point
is `Manager`; creating it opens the existing native capture context. Dispose it when done.

The driver must be obtained and installed separately. Installing this NuGet package
does not install a driver or create a capture session. Driver demo restrictions and
licensing remain unchanged. The native DLLs are copied beside the application under
`x64/` and `x86/` to match the SDK's existing loader.

Build the package with `dotnet pack -c Release`. NuGet.org publication requires the
owner's account and credentials. Source: https://github.com/TitanCSA/AutoHotInterceptor

## Install from NuGet

```sh
dotnet add package AutoHotInterceptor --version 1.0.1
```

[Source and API](https://github.com/TitanCSA/AutoHotInterceptor)
