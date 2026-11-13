# AutoHotInterceptor PowerShell SDK

`Install-Module AutoHotInterceptor -Scope CurrentUser`

Requires PowerShell 7.4+ on Windows x64. Supported families: keyboard, mouse. The native product SDK and license/trial behavior are unchanged. No driver is installed on import or construction.

```powershell
Import-Module AutoHotInterceptor
$client = Connect-AutoHotInterceptorSDK
try { Get-AutoHotInterceptorSDKInfo -Client $client }
finally { Disconnect-AutoHotInterceptorSDK -Client $client }
```

See [API guide](docs/API.md) and `Get-Command -Module AutoHotInterceptor`. The matching driver is installed separately.
