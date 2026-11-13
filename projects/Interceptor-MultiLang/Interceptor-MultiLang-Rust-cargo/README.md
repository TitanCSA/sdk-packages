# Interceptor MultiLang for Rust

Windows x64 Rust bindings for the combined Interceptor input driver.
Requires Rust 1.82+, Windows x64 and the matching Interceptor 1.4.22.0 driver.
The native DLL and import library are bundled; no kernel driver is installed by Cargo.

```sh
cargo add interceptor-multilang
```

The library import name is `interceptor`:

```rust
use interceptor::Interceptor;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut io = Interceptor::new()?;
    io.close()?;
    Ok(())
}
```

The build script copies the native DLL into Cargo's target output. When distributing
your executable, place the matching `InterceptorBridge.dll` beside it.

Capture requires a separately installed matching driver and an administrator
terminal. Always forward complete received input batches or explicitly mark
blocked entries. Closing the context releases capture. The driver's demo and
licensing restrictions remain separate from this MIT wrapper.

API reference: https://github.com/TitanCSA/Interceptor-MultiLang/blob/main/docs/API.md
Source: https://github.com/TitanCSA/Interceptor-MultiLang
Bundled documentation: `docs/API.md`; example: `examples/info.rs`.
