# Interceptor MultiLang for Ruby

Direct Fiddle bindings for Windows x64 Ruby 3.1+, with the native DLL bundled.
No Python runtime, compiler or subprocess bridge is required.

```sh
gem install interceptor-multilang
```

The installation command is available after RubyGems publication.

```ruby
require 'interceptor_multilang'
InterceptorMultiLang::Interceptor.open do |io|
  p io.info
end
```

Opening a client is lazy. For capture, separately install the combined Interceptor
1.4.22.0 driver and run the script as administrator. No driver is installed by gem.
Only one native SDK context may be open per process; use the block form or close
the client in `ensure`. Receive is synchronous and releases Ruby's GVL; cancel
can interrupt a receive from another Ruby thread.

```ruby
batch = io.receive(timeout: 1000)
batch.each do |input|
  input.field('y').value *= -1 if input.kind == 'mouse'
end
io.send(batch)
```

Always forward the complete batch or deliberately block/discard events.
`batch[0].blocked = true` suppresses an event. A new receive invalidates older
batches, including failed receives. Partial-send errors expose the accepted count;
retrying `send` on the same current batch forwards only the remainder.

The API includes `initialize_devices`, `info`, `devices`, `receive`, `send`,
`discard`, `cancel`, `descriptor(token, report: true)`,
`inject('mouse', {'x' => 200})` and `close`.
HID fields retain collection, usage and element metadata; duplicate names are not
collapsed. Sequence/timestamp values are exact Ruby integers. Binary data is a byte
string; its native length must not change when editing a received field.

[API and event fields](docs/API.md), examples and MIT [LICENSE](LICENSE) are bundled.
Driver demo restrictions and licensing remain separate from this SDK.
