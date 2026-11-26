# Titan-Audio-Link Rust SDK

`cargo add titan-audio-link@=1.0.1`

For the standalone example, see the [Rust ZIP quickstarts](https://github.com/TitanCSA/sdk-packages/releases/tag/rust-quickstarts-v1.0.1).

Windows x64 MSVC Rust1.82+. Bundled DLL/import library; copy native DLL beside deployed executable.

```rust,no_run
use titan_audio_link::Client;
let mut client=Client::new()?;
let result=client.call("list",serde_json::json!({}))?;
# Ok::<(),Box<dyn std::error::Error>>(())
```

[API](docs/API.md). Drop closes owned session; original driver and licensing requirements unchanged.

Version 1.0.1 stages the native DLL for `cargo run` and `cargo test` with Cargo's default build layout. For a standalone executable or a build directory outside the target directory, place the matching DLL from this crate's `native/` directory next to the executable.
