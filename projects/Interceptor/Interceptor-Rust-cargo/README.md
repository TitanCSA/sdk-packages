# Interceptor Rust SDK

`cargo add interceptor-sdk@=1.0.1`

Windows x64 MSVC Rust 1.82+. The product DLL/import library is bundled. Copy `native/InterceptorSDKBridge.dll` beside a deployed executable. No driver is installed or capture started during package installation or construction.

```rust,no_run
use interceptor_sdk::Interceptor;
let mut client = Interceptor::new()?;
let info = client.info()?;
# Ok::<(), Box<dyn std::error::Error>>(())
```

[API](docs/API.md). Native source: product repository C-ffi directory. Original licensing/driver prerequisites remain unchanged.

Version 1.0.1 stages the native DLL for `cargo run` and `cargo test` with Cargo's default build layout. For a standalone executable or a build directory outside the target directory, place the matching DLL from this crate's `native/` directory next to the executable.
