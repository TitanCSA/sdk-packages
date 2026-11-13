# Interceptor MultiLang for Dart and Flutter Windows

Direct `dart:ffi` bindings for Windows x64. The native DLL is bundled; no Python,
compiler, external executable or third-party Dart package is required at runtime.

```sh
dart pub add interceptor_multilang
```

For Flutter desktop use `flutter pub add interceptor_multilang`. These commands
become available after pub.dev publication. Windows x64 and Dart 3.4+ are required.

```dart
import 'package:interceptor_multilang/interceptor_multilang.dart';

Future<void> main() async {
  final io = await Interceptor.open();
  try { print(io.info()); } finally { io.close(); }
}
```

The DLL is resolved from this package's `lib/native` directory. A standalone
compiled app may pass an explicit `libraryPath` to its deployed DLL.
Separately install the combined Interceptor 1.4.22.0 driver for capture and run
the capture process as administrator. Package installation never installs a driver.

## Capture and Flutter isolates

Native calls, including `receive`, are synchronous. Create the SDK client and run
the capture loop in a worker isolate, rather than on Flutter's UI isolate. Use a
finite receive timeout so the worker can process control messages between batches.
Only one SDK context may be open per process. Always close it in `finally`.

```dart
final batch = io.receive(timeout: 1000);
for (final input in batch.events) {
  if (input.kind == 'mouse') input.field('y').value *= -1;
}
io.send(batch);
```

Always send the complete batch or explicitly block/discard events. Batches retain
their owner/ticket and become invalid after the next receive, including a failed
receive. Repeating a partial send forwards only its unaccepted remainder.
Errors expose `code`, `message` and `accepted`. Repeated HID fields stay separate
with collection/usage metadata; ambiguous name lookups are rejected.
Unsigned sequence, timestamp and large info counters use `BigInt` to preserve
64 bits. Signed state values use `int`. Binary/report values use `Uint8List`;
do not resize a received binary field.

API: `initialize`, `info`, `devices`, `receive`, `send`, `discard`, `cancel`,
`descriptor(token, report: true)`, `inject('mouse', {'x': 200})`, `close`.
[Full event and descriptor guide](doc/API.md), an example and MIT [LICENSE](LICENSE)
are bundled. Driver demo restrictions and licensing remain separate.
