# node-interceptor Rust SDK

`cargo add node-interceptor`

Windows x64 MSVC Rust1.82+. Bundled DLL/import library; copy native DLL beside deployed executable.

```rust,no_run
use node_interceptor::Client;
let mut client=Client::new()?;
let result=client.call("check",serde_json::json!({"device":1}))?;
# Ok::<(),Box<dyn std::error::Error>>(())
```

[API](docs/API.md). Drop closes owned session; original driver and licensing requirements unchanged.
