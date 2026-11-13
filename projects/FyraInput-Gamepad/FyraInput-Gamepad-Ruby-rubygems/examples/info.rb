require 'fyrainput_gamepad'
FyraInputGamepad::Interceptor.open { |client| p client.info }
