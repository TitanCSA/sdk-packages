# Interceptor MultiLang for PHP

Direct PHP FFI bindings for Windows x64, with the native bridge bundled.
Requires PHP 8.2+ x64, the FFI extension, and `ffi.enable=true` for your CLI app.
No Python runtime or subprocess bridge is used.

```sh
composer require titancsa/interceptor-multilang
```

The Composer command is available after this repository is registered on Packagist.

```php
require 'vendor/autoload.php';
use TitanCSA\Interceptor\Interceptor;
$io = new Interceptor();
try {
    print_r($io->info());
} finally {
    $io->close();
}
```

For capture, separately install the combined Interceptor 1.4.22.0 driver and run
your CLI as administrator. Installing the Composer package does not install a driver.
One SDK context may be open per process. Always close it in a `finally` block.

`receive(1000)` waits up to 1000 ms and returns a `Batch`. Edit copied field values
and `blocked` flags, then call `send($batch)` on the complete batch. A subsequent
receive invalidates the previous batch, including when receive fails. `discard`
intentionally suppresses a received batch. `send` retry forwards only the remaining
events after a partial native send; exceptions expose `accepted` and the Win32 code.

```php
$batch = $io->receive(1000);
foreach ($batch as $input) {
    if ($input->kind === 'mouse') $input->field('y')->value *= -1;
}
$io->send($batch);
```

`initialize()`, `info()`, `devices()`, `descriptor($token, report: true)`,
`inject('mouse', ['x' => 200])`, `cancel()` and `close()` are also available.
Repeated HID fields stay separate in `Input::$fields`, with collection/usage/index
metadata. `field(name)` rejects ambiguous names. Unsigned sequence/timestamp/info
64-bit values are decimal strings, preserving all bits in PHP. Signed state values
are PHP 64-bit integers. Do not resize binary/report fields. Calls are synchronous.

The package includes [API/event fields](docs/API.md), examples and MIT [LICENSE](LICENSE).
The driver's demo restrictions and licensing remain separate from this SDK.
