import 'package:interceptor_multilang/interceptor_multilang.dart';

Future<void> main() async {
  final io = await Interceptor.open();
  try {
    print(io.info());
  } finally {
    io.close();
  }
}
