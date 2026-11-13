# FyraDevice Ruby SDK

`gem install fyradevice`

Windows x64 Ruby3.1+.

```ruby
require "fyradevice"
FyraDeviceSDK::Client.open { |client| p client.call("status") }
```

[API and operation schema](docs/API.md). Matching driver installed separately.
