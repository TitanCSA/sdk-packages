# Validation

Validation on 30 October 2026 covers the native binding and this driver's input path; it does not promise support for every Windows device or application.

- Synthetic native x64/x86 tests cover routing, filter flags, signed mouse fields, identity separation, overflow, invalid handles and repeated create/destroy. The shared bridge also passes MSVC AddressSanitizer.
- JS/TypeScript tests cover API exports, endpoint IDs, 2,000 asynchronous receive/forward iterations, invalid argument ranges/types, context cleanup, cancellation, closure during a pending wait and explicit unsupported precedence.
- Node.js 20.20.2, 22.23.4 and 24.21.0 x64 are exercised. x86 prebuild loading is checked with Node.js 22.23.4 on x64 Windows. Node-API 8 is used, but every Electron build has not been qualified.
- Worker termination is stress-tested through 100 separate process/Worker lifecycles for each tested x64 Node major. Async waiting uses a Windows wait registration and Node-API completion callback; an idle wait does not occupy libuv's worker pool or poll in JS.
- A disposable Windows 11 VM uses the unchanged signed driver and four owned HID sources (two keyboards, two mouse devices). JS receives their separate source IDs, remaps A to B and inverts mouse Y. An independent Windows Raw Input observer confirms the transformed input downstream.

These checks are not input-to-photon latency measurements. JS execution, garbage collection, slow callbacks and UI/console logging can delay selected events. Use `finally`/`destroy()` and handle native errors. No test-only injection exports are included in production addons.
