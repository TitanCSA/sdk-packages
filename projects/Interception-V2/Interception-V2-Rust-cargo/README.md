# Interception-V2 Rust SDK

Use the public Rust SDK guide for distribution options. To use this source locally, run `cargo add --path PATH_TO_THIS_CRATE` in your Rust project.

Windows x64 MSVC Rust1.82+. Bundled DLL/import library; copy native DLL beside deployed executable.

```rust,no_run
use interception_v2::Client;
let mut client=Client::new()?;
let result=client.call("check",serde_json::json!({"device":1}))?;
# Ok::<(),Box<dyn std::error::Error>>(())
```

[API](docs/API.md). Drop closes owned session; original driver and licensing requirements unchanged.

Version 1.0.1 stages the native DLL for `cargo run` and `cargo test` with Cargo's default build layout. For a standalone executable or a build directory outside the target directory, place the matching DLL from this crate's `native/` directory next to the executable.
