# API and examples

The short examples in `examples/` invert mouse Y and forward every other input unchanged. Install the matching driver and run them as administrator. `initialize()` is available for newly connected devices; check `ready` and `reboot_required` before capturing.

## Shared behavior

`receive` waits in milliseconds. Python/Node default `None`/`null`, Rust `None`, and .NET/Java/Go `-1` mean infinite. Zero is nonblocking. Timeout or cancellation returns an empty batch. Native C uses `UINT32_MAX` for infinite.

Receive consumes input. Until the batch is sent or discarded, another receive is rejected. A batch has fixed membership and source order: edit its state or set `blocked`; do not resize/reorder it. Send infers counts and returns the number forwarded. Blocked events are excluded. On partial failure, the error includes `accepted`; retry the same batch to send only the remaining entries. There is no hidden retry loop.

Native errors carry a Windows code and an English explanation. Python exposes `Error.code/accepted`, Node errors have `code/accepted`, .NET has `InterceptorException.Code/Accepted`, Java `Interceptor.Error.code/accepted`, Go `*interceptor.Error`, and Rust `interceptor::Error`. After a driver revocation, close the client before explicitly starting a new capture; an expired demo still requires Windows Restart.

Managed input objects are detached snapshots. Their native commit references are checked against the client and current batch generation before use. Closing a client or attempting a new receive can invalidate the prior batch's send ticket. Ordinary state inspection remains safe; sending an old batch raises an error. Native C views are borrowed: use the lifetime rules in the header and do not access views concurrently with receive/close.

## Input identity

Each input has a kind (`keyboard`, `mouse`, `hid`, `xinput`, `xinputhid`, `gip`), a capture-scoped string `token`, sequence, QPC timestamp and a mutable blocked flag. Metadata is read-only. Tokens are not persistent hardware serial numbers; obtain them again for each capture. Sequence/timestamp are 64-bit: JavaScript uses BigInt; Java `long` preserves their unsigned bit pattern.

`devices()` lists sources from the driver's capture, not every PnP child node. It opens capture and starts the demo allowance if not already started. `info()` is a read-only query. Its `active` field reports driver-wide capture activity, while `device_count` is the size of this client's last device enumeration (zero before enumeration). Its legacy `queued` field covers keyboard, mouse and HID queues; it is not an all-transport queue total. Use the returned batches for actual input activity.

## State fields

| Kind | Scalar fields |
| --- | --- |
| Keyboard | `scan_code`, `flags`, `information` |
| Mouse | `x`, `y`, `flags`, `buttons`, signed `wheel`, `raw_buttons`, `information` |
| XInput / GIP | read-only `packet`, `buttons`, `left_trigger`, `right_trigger`, `left_x`, `left_y`, `right_x`, `right_y`, `extra_buttons` |

Keyboard flags preserve make/break and extended-key information. Mouse coordinates are relative unless the native absolute flag is set. Mouse `buttons` contains transition flags; gamepad `buttons` is a state bitmask. XInput and GIP use their native button positions and trigger ranges: XInput 0-255, GIP 0-1023, stick axes signed 16-bit. Do not interchange the protocols' button masks.

HID/XInputHID expose dynamic `axes`, `triggers`, `hats`, `values`, `buttons`, and `binary_values`, plus the complete report. Python/Node use `input.state.axes`; .NET uses `input.State.Axes`; Java uses `input.state.axes`; Rust uses `input.state.axes`; Go uses `input.HID.Axes`. Scalar maps are `state` in Python/Node, `State` in .NET/Go, `state.scalars` in Java/Rust.

HID scalar fields include raw value, minimum/maximum, physical range, normalized value and validity, usage page/usage, collection, report ID, data index and element. Repeated X usages remain distinct. Button `pressed` is mutable. Wide values retain their packed bytes in `data`. Flags: 1 normalized valid, 2 relative, 4 has null, 8 is null, 16 identified trigger, 32 read-only.

Edit a scalar's **raw value or normalized value**. Absolute axis normalization is -1..1; identified analog triggers use 0..1. Relative/null/constant-range values may have no valid normalization. An unchanged null hat may be outside its declared ordinary angle range and is preserved. Contradictory edits or values outside valid ranges are rejected. Report and binary data lengths are immutable.

HID state is the current report. Check `decode_complete` and `decode_error` before relying on decoded controls. Unknown bytes remain in the original report when forwarded unchanged; proprietary meaning is not guessed.

## Injection and descriptors

Create input for an existing captured source with `inject`. Supplying a token selects that exact source. Omitting it selects the first device of the requested kind. The normal send operation always retains the original captured source.

```python
with Interceptor() as io:
    mouse = next(d for d in io.devices() if d.kind == "mouse")
    io.inject("mouse", token=mouse.token, x=200, y=0)
```

Python supports named state keywords; Node accepts `{kind, token?, state, report?}`; .NET accepts `Inject(kind, stateDictionary, token?, report?)`; Java accepts `inject(kind, stateMap, token, report)`; Rust accepts kind, a slice of name/value pairs, optional token and optional report. Go uses a state map and an optional `Injection{Token: ..., Report: ...}`. Raw HID injection requires a valid report for that device. GIP synthetic input uses named standard state; captured GIP preserves its original complete report and extensions.

`descriptor(token, report=false)` returns common descriptor data. `report=true` requests the **actual raw HID report descriptor**, where accessible. XUSB/GIP return not applicable. An unsupported descriptor transport returns an error; the wrapper does not substitute Windows preparsed data for a raw HID report descriptor. Formats: 0 identity, 1 HID preparsed data, 2 GIP metadata, 3 raw HID report descriptor.

## Java

From the extracted release folder, using JDK 22+:

```powershell
javac -cp bindings/java/Interceptor.jar -d examples/java examples/java/Remap.java
java --enable-native-access=ALL-UNNAMED -cp "bindings/java/Interceptor.jar;examples/java" Remap
```

The native-access flag is required by the JDK's Foreign Function and Memory API. No JNI build or third-party Java dependency is needed. Keep the release's `runtimes/win-x64/` folder, or place the DLL beside the JAR.

## Add to another application

- Python: install the bundled Windows x64 wheel in the application's environment.
- Node/TypeScript: `npm install interceptor-multilang`, then import `interceptor-multilang`.
- .NET: use the local `TitanCSA.Interceptor` NuGet package or reference `bindings/dotnet/Interceptor.csproj`.
- Java/Kotlin: add `bindings/java/Interceptor.jar` to the classpath.
- Go: import the source module in `bindings/go`; distribute `InterceptorBridge.dll` beside the final EXE.
- Rust: use a path dependency on `bindings/rust`. Its build script links the import library and copies the DLL beside the executable. `INTERCEPTOR_LIB_DIR` can select another directory containing matching DLL/LIB files.
- C: link `InterceptorBridge.lib`, include `InterceptorBridge.h` and distribute the DLL beside the EXE.
- C++: compile `sdk/Interceptor.cpp` once and include `sdk/Interceptor.h`; the unchanged upstream SDK remains available directly.

For development/testing, `INTERCEPTOR_NATIVE_PATH` can explicitly select a matching native DLL. Node uses `INTERCEPTOR_NODE_PATH` for a matching addon. Do not select the synthetic test backend for normal capture. Paths are resolved to supplied files; installation does not add global PATH entries or a wrapper service.
