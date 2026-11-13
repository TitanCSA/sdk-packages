# Build and test

Install Visual Studio 2022 C++ tools, the .NET Framework 4.8 targeting pack, Windows SDK and CMake 3.24+.

```powershell
powershell -ExecutionPolicy Bypass -File .\Build.ps1
ctest --test-dir obj/x64 -C Release --output-on-failure
ctest --test-dir obj/x86 -C Release --output-on-failure
```

The native tests use a **synthetic SDK**, never the installed driver or physical input. `InterceptorBridgeFixture` is a test-only DLL; it is not shipped in either production `Lib` folder. `ManagedTests.cs` exercises subscriptions, device separation, context callbacks, callback failures and scan-code round trips against that fixture. `Smoke-v1.ahk` / `Smoke-v2.ahk` exercise the stock runtimes with the same fixture; execute them only in an isolated fixture directory.

Production native DLLs use static MSVC runtime linkage. C# remains a .NET Framework assembly loaded by the original CLR bridge. Include each version's complete `Lib` folder when distributing scripts.

Stock AutoHotkey binaries are unmodified. `licenses/AutoHotkey-Runtimes.json` records exact versions, official download/source URLs and hashes. The release supplies their corresponding sources separately. Runtime updates should verify upstream release hashes and retain their GPL license/source distribution.

The existing signed driver installers are redistributed byte-for-byte with their binary license. Building this project does not build or re-sign a kernel driver.
