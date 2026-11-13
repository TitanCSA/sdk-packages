require 'fyrainput_keyboard_mouse'
FyraInputKeyboardMouse::Interceptor.open { |client| p client.info }
