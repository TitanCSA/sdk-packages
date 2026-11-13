# Titan-Audio-Link PHP SDK

`composer require titancsa/titan-audio-link`

Windows x64 PHP8.2+; enable FFI extension and `ffi.enable=true`.

```php
require "vendor/autoload.php";
$client=new \TitanCSA\Products\TitanAudioLink\Client();
try {print_r($client->call("list"));} finally {$client->close();}
```

[API and operation schema](docs/API.md). Matching driver installed separately.
