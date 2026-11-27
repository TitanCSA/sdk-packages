# Interception V2 PowerShell SDK

Use PowerShell 7.4 or newer on 64-bit Windows.

```powershell
Install-Module InterceptionV2 -Repository PSGallery -RequiredVersion 1.0.0 -Scope CurrentUser
Import-Module InterceptionV2 -RequiredVersion 1.0.0
$client = Connect-InterceptionV2SDK
try {
    Invoke-InterceptionV2SDK -Client $client -Operation check -Arguments @{ device = 1 }
} finally {
    Disconnect-InterceptionV2SDK -Client $client
}
```

The check returns `keyboard: true` for slot 1. It can run before driver setup. Install the driver separately and follow the [Interception V2 first-run guide](https://titancsa.github.io/guides/interception-v2-migration/) to check devices connected to Windows.

Use the [API reference](docs/API.md) to build the argument hashtable. Native errors carry the Windows error code in the inner exception.

The Gallery 1.0.0 package includes the [LGPL 3.0 terms](LICENSE). The [original license text](https://github.com/TitanCSA/Interception-V2/blob/main/licenses/non-commercial-usage/LGPL%203.0.txt) matches that copy.
