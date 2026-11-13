# USB-over-IP-SDK PHP SDK

`composer require titancsa/usb-over-ip-sdk`

Windows x64 PHP8.2+; enable FFI extension and `ffi.enable=true`.

```php
require "vendor/autoload.php";
$client=new \TitanCSA\Products\USBoverIPSDK\Client();
try {print_r($client->call("client_status"));} finally {$client->close();}
```

[API and operation schema](docs/API.md). Matching driver installed separately.
