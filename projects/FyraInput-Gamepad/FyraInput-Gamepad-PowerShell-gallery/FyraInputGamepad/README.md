# FyraInput-Gamepad PowerShell SDK

`Install-Module FyraInputGamepad -Scope CurrentUser`

Requires PowerShell 7.4+ on Windows x64. Supported families: hid, xinput, xinputhid, gip. The native product SDK and license/trial behavior are unchanged. No driver is installed on import or construction.

```powershell
Import-Module FyraInputGamepad
$client = Connect-FyraInputGamepadSDK
try { Get-FyraInputGamepadSDKInfo -Client $client }
finally { Disconnect-FyraInputGamepadSDK -Client $client }
```

See [API guide](docs/API.md) and `Get-Command -Module FyraInputGamepad`. The matching driver is installed separately.
