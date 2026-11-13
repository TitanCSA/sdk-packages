# node-interceptor PowerShell SDK

PowerShell7.4+ Windows x64. `Install-Module NodeInterceptor -Scope CurrentUser`.

```powershell
$client=Connect-NodeInterceptorSDK
try { Invoke-NodeInterceptorSDK -Client $client -Operation check -Arguments @{device=1} } finally { Disconnect-NodeInterceptorSDK -Client $client }
```

Use -Arguments with a hashtable matching [schema](docs/API.md). Native errors retain their Windows code through the inner exception. Matching driver installed separately.
