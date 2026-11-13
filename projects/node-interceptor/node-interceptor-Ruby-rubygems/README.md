# node-interceptor Ruby SDK

`gem install node-interceptor`

Windows x64 Ruby3.1+.

```ruby
require "node_interceptor"
NodeInterceptorSDK::Client.open { |client| p client.call("check", {"device" => 1}) }
```

[API and operation schema](docs/API.md). Matching driver installed separately.
