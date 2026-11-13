import 'dart:io';
import 'package:interceptor_multilang/interceptor_multilang.dart';

void check(bool condition, String message) {
  if (!condition) throw StateError(message);
}

Future<void> main() async {
  final io = await Interceptor.open();
  try {
    if (Platform.environment['INTERCEPTOR_TEST_BACKEND'] == '1') {
      check(io.initialize()['ready'] == 1, 'initialize');
      final devices = io.devices();
      check(devices.length == 6, 'devices');
      check((devices[0]['name'] as String).contains('\u00e9'), 'UTF-8');
      final batch = io.receive(timeout: 0);
      check(batch.length == 6, 'six families');
      check(batch[0].sequence == BigInt.parse('9223372036854775809'), 'uint64');
      batch[0].field('scan_code').value = 0x30;
      batch[1].field('y').value *= -1;
      final axes =
          batch[2].fields.where((f) => f.metadata['group'] == 1).toList();
      check(axes.length == 2, 'repeated HID');
      axes[0].value = 50;
      axes[0].normalized = .5;
      batch[5].blocked = true;
      check(io.send(batch) == 5, 'send/block');
      check(io.send(batch) == 0, 'retry');
      check(io.inject('mouse', {'x': 200}) == 1, 'inject');
      final newer = io.receive(timeout: 0);
      var rejected = false;
      try {
        io.send(batch);
      } on ArgumentError {
        rejected = true;
      }
      check(rejected, 'stale batch');
      io.discard(newer);
      io.cancel();
      io.close();
      final other = await Interceptor.open();
      try {
        rejected = false;
        try {
          other.send(newer);
        } on ArgumentError {
          rejected = true;
        }
        check(rejected, 'foreign batch');
      } finally {
        other.close();
      }
      print(
          'Dart native fixture passed: six families, UTF-8, uint64, edits, HID, send/block, retry, injection, stale/foreign batches');
    } else {
      print('Dart bundled production native DLL lazy open/close passed');
    }
  } finally {
    io.close();
  }
}
