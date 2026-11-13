# Interceptor MultiLang PowerShell SDK

Requires Windows x64 and PowerShell 7.4+. The matching combined Interceptor
1.4.22.0 driver must be installed separately for capture. Importing this module
and opening a context do not install a driver or start input capture.

```powershell
Install-Module InterceptorMultiLang -Scope CurrentUser
Import-Module InterceptorMultiLang
$io = Connect-Interceptor
try {
    Get-InterceptorInfo -Client $io
} finally {
    Disconnect-Interceptor -Client $io
}
```

The Install-Module command is available after PowerShell Gallery publication.
For a local copy use `Import-Module ./InterceptorMultiLang/InterceptorMultiLang.psd1`.
This SDK uses the bundled .NET assembly and native DLL directly; Python is not required.

See [all cmdlets](docs/COMMANDS.md), [API fields](docs/API.md) and the bundled example.
Capture requires an administrator session. Always forward the complete received
batch, or explicitly mark blocked events. Disposing releases capture.
The driver's demo restrictions and licensing remain separate from this MIT wrapper.
