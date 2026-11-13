import 'package:autohotinterceptor/autohotinterceptor.dart';

Future<void> main() async {
  final client = await Interceptor.open();
  try {
    print(client.info());
  } finally {
    client.close();
  }
}
