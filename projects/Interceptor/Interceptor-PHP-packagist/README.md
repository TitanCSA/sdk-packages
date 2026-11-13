# Interceptor PHP SDK

`composer require titancsa/interceptor`

PHP 8.2+ Windows x64, extension FFI and `ffi.enable=true` are required. Matching driver installed separately; licensing and trial rules remain unchanged. Supported families: keyboard, mouse, hid, xinput, xinputhid, gip.

```php
require "vendor/autoload.php";
$client = new \TitanCSA\Products\Interceptor\Interceptor();
try { print_r($client->info()); } finally { $client->close(); }
```

See [API](docs/API.md). Installation and construction do not start input capture.
