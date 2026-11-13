import 'package:fyrainput_keyboard_mouse/fyrainput_keyboard_mouse.dart';

Future<void> main() async {
  final client = await Interceptor.open();
  try {
    print(client.info());
  } finally {
    client.close();
  }
}
