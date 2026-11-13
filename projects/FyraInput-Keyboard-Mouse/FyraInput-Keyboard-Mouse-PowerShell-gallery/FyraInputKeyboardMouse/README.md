# FyraInput-Keyboard-Mouse PowerShell SDK

`Install-Module FyraInputKeyboardMouse -Scope CurrentUser`

Requires PowerShell 7.4+ on Windows x64. Supported families: keyboard, mouse. The native product SDK and license/trial behavior are unchanged. No driver is installed on import or construction.

```powershell
Import-Module FyraInputKeyboardMouse
$client = Connect-FyraInputKeyboardMouseSDK
try { Get-FyraInputKeyboardMouseSDKInfo -Client $client }
finally { Disconnect-FyraInputKeyboardMouseSDK -Client $client }
```

See [API guide](docs/API.md) and `Get-Command -Module FyraInputKeyboardMouse`. The matching driver is installed separately.
