require 'autohotinterceptor'
AutoHotInterceptor::Interceptor.open { |client| p client.info }
