# FyraDevice PowerShell SDK

PowerShell7.4+ Windows x64. `Install-Module FyraDevice -Scope CurrentUser`.

```powershell
$client=Connect-FyraDeviceSDK
try { Invoke-FyraDeviceSDK -Client $client -Operation status } finally { Disconnect-FyraDeviceSDK -Client $client }
```

Use -Arguments with a hashtable matching [schema](docs/API.md). Native errors retain their Windows code through the inner exception. Matching driver installed separately.
