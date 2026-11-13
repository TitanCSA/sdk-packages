# PHP API

`new Interceptor(?string $libraryPath = null)`; always call `close()` in finally. `initialize`, `info`, `devices`, `receive(?int $timeout = null)`, `send(Batch $batch)`, `discard(Batch $batch)`, `cancel`, `descriptor(string $token, bool $report = false)` and `inject(string $kind, array $state = [], ?string $token = null, ?string $report = null)` call the product native SDK.

Receive consumes a batch: forward with send or explicitly discard. Edit `event->field(name)->value`, `normalized`, `data`, or `event->blocked`. Descriptor metadata disambiguates repeated HID fields. Stale, foreign, resized and reordered batches are rejected. NativeException exposes accepted count for retry after a partial send. Unsigned 64-bit sequence/timestamps are decimal strings, preserving all bits in PHP. Unsupported families return Windows error 50.

The PHP client is synchronous; `cancel` can interrupt a native receive through a separate native thread, but this package does not create a PHP worker/thread runtime. Tested using the product-specific synthetic backend and production lazy open/close. Native C adapter source is in the original product repository.
