# Browser-Control-SDK PHP SDK

`composer require titancsa/browser-control-sdk`

Windows x64 PHP8.2+; enable FFI extension and `ffi.enable=true`.

```php
require "vendor/autoload.php";
$client=new \TitanCSA\Products\BrowserControlSDK\Client();
try { $client->call("connect"); print_r($client->call("info"));} finally {$client->close();}
```

[API and operation schema](docs/API.md). Running BrowserControl application installed separately.

Before requesting browser information, call the `connect` operation with an empty argument object (discover an existing instance) or `{endpoint: "absolute path"}`. Construction alone does not connect or launch a browser. The `request` operation takes `{method, params, timeout}` and exposes the original browser API. The app must be installed and running separately.
