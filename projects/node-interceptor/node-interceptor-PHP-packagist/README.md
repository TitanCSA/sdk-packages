# node-interceptor PHP SDK

`composer require titancsa/node-interceptor`

Windows x64 PHP8.2+; enable FFI extension and `ffi.enable=true`.

```php
require "vendor/autoload.php";
$client=new \TitanCSA\Products\NodeInterceptor\Client();
try {print_r($client->call("check", ["device" => 1]));} finally {$client->close();}
```

[API and operation schema](docs/API.md). Matching driver installed separately.
