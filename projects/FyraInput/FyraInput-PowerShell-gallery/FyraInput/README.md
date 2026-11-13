# FyraInput PowerShell SDK

`Install-Module FyraInput -Scope CurrentUser`

Requires PowerShell 7.4+ on Windows x64. Supported families: keyboard, mouse, hid, xinput, xinputhid, gip. The native product SDK and license/trial behavior are unchanged. No driver is installed on import or construction.

```powershell
Import-Module FyraInput
$client = Connect-FyraInputSDK
try { Get-FyraInputSDKInfo -Client $client }
finally { Disconnect-FyraInputSDK -Client $client }
```

See [API guide](docs/API.md) and `Get-Command -Module FyraInput`. The matching driver is installed separately.
