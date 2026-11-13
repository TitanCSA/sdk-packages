require 'fyrainput'
FyraInput::Interceptor.open { |client| p client.info }
