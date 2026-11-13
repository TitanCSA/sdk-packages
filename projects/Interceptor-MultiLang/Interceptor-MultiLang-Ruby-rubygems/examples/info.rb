require 'interceptor_multilang'
InterceptorMultiLang::Interceptor.open { |io| p io.info }
