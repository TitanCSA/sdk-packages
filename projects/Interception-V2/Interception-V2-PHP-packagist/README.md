# Interception-V2 PHP SDK

`composer require titancsa/interception-v2`

Windows x64 PHP8.2+; enable FFI extension and `ffi.enable=true`.

```php
require "vendor/autoload.php";
$client=new \TitanCSA\Products\InterceptionV2\Client();
try {print_r($client->call("check", ["device" => 1]));} finally {$client->close();}
```

[API and operation schema](docs/API.md). Matching driver installed separately.
