# AutoHotkey API

Create one `AutoHotInterception` object per script. In v2 use `AHI := AutoHotInterception()`; in v1 use `AHI := new AutoHotInterception()`. Its `.Instance` is the managed wrapper for advanced callers. The adapter preserves the upstream interface documented below. `GetLastError()` is the added English diagnostic accessor.

## Find a device

| Call | Result |
| --- | --- |
| `GetDeviceList()` | Map of IDs to `{ID, VID, PID, IsMouse, Handle}` |
| `GetDeviceId(isMouse, vid, pid, instance := 1)` | Current compatibility ID |
| `GetKeyboardId(vid, pid, instance := 1)` | Keyboard ID |
| `GetMouseId(vid, pid, instance := 1)` | Mouse ID |
| `GetDeviceIdFromHandle(isMouse, handle, instance := 1)` | Match the full hardware ID |
| `GetKeyboardIdFromHandle(handle, instance := 1)` | Keyboard hardware-ID lookup |
| `GetMouseIdFromHandle(handle, instance := 1)` | Mouse hardware-ID lookup |

The AutoHotkey wrapper reports missing selections and exits rather than subscribing to ID zero. VID/PID identifies a model, not necessarily one physical unit. `instance` distinguishes duplicate matches. IDs are 1–10 / 11–20 for the capture; do not save them as permanent identities. `Handle` retains the original API name; it is a hardware-ID string, not an OS handle.

## Subscribe

| Call | Callback |
| --- | --- |
| `SubscribeKey(id, code, block, callback, concurrent := false)` | `(state)` |
| `SubscribeKeyboard(id, block, callback, concurrent := false)` | `(code, state)` |
| `SubscribeMouseButton(id, button, block, callback, concurrent := false)` | `(state)` |
| `SubscribeMouseButtons(id, block, callback, concurrent := false)` | `(button, state)` |
| `SubscribeMouseMoveRelative(id, block, callback, concurrent := false)` | `(x, y)` |
| `SubscribeMouseMoveAbsolute(id, block, callback, concurrent := false)` | `(x, y)` |
| `SubscribeMouseMove(...)` | Alias of the relative movement method |

`code` uses AutoHotkey scan codes, including extended codes returned by `GetKeySC()`. Press is **1**, release **0**. Mouse button indices **0–4** mean left/right/middle/X1/X2; **5** is vertical wheel, **6** horizontal wheel. Wheel state is its direction **-1 or +1**. Raw fractional wheel magnitudes are normalized to directions by the inherited AutoHotkey API; use the C++ or Node stroke API for exact rolling values.

`block=true` suppresses the subscribed physical event. Other keys/button fields are forwarded. A mouse packet may contain movement and buttons together: blocking one component leaves the others available. By default each subscription queues callbacks sequentially; `concurrent=true` schedules independent callbacks and does not guarantee completion order. AutoHotkey itself still has its own thread/message execution rules.

Unsubscribe with `UnsubscribeKey(id, code)`, `UnsubscribeKeyboard(id)`, `UnsubscribeMouseButton(id, button)`, `UnsubscribeMouseButtons(id)`, `UnsubscribeMouseMoveRelative(id)`, `UnsubscribeMouseMoveAbsolute(id)` or the relative alias `UnsubscribeMouseMove(id)`.

`SetState(false)` disables all active subscriptions/context filtering and restores passthrough while retaining their definitions. `SetState(true)` enables them again. This is not a demo timer reset.

## Context-sensitive standard hotkeys

```autohotkey
AHI := AutoHotInterception()
keyboard := AHI.GetKeyboardId(0x1234, 0x5678)
context := AHI.CreateContextManager(keyboard)

#HotIf context.IsActive
F1::MsgBox("F1 from this keyboard")
#HotIf
```

Use `#if context.IsActive` in v1. Ordinary keyboard/button events are forwarded with the context enabled around delivery; context mode does not provide a mouse-movement hotkey. Remove it with `RemoveContextManager(id)`. Do not create two context managers for the same ID.

## Send

| Call | Meaning |
| --- | --- |
| `SendKeyEvent(id, code, state)` | Scan-code press/release (1/0) |
| `SendMouseButtonEvent(id, button, state)` | Button or wheel direction |
| `SendMouseButtonEventAbsolute(id, button, state, x, y)` | Button with absolute coordinates |
| `SendMouseMoveRelative(id, x, y)` / `SendMouseMove(id, x, y)` | Signed relative device movement |
| `SendMouseMoveAbsolute(id, x, y)` | Normalized absolute 0–65535 coordinates |
| `MoveCursor(x, y, coordinateMode := "Screen", mouseId := -1)` | Inherited cursor-position helper using relative steps; not a precise timing primitive |

Send operations target an existing source device; they do not create a virtual mouse/keyboard. Driver send failures become managed exceptions. The public calls are procedures, so successful calls do not return a count. `GetLastError()` returns the latest asynchronous capture/callback failure, or an empty string before any failure. Native capture is released on a fatal receive/forward error. Close the script to finish.

## Packaging and limits

Keep `AutoHotInterception.ahk`, `CLR.ahk`, `AutoHotInterception.dll`, and the `x64`/`x86` native subdirectories together under `Lib`. The normal upstream `FileInstall` branches remain available for compiled scripts. Use the matching AutoHotkey major version and trusted local files.

The driver demo, one-owner limit, scope of IDs and supported architectures are described in the main README. This preserves the scripting workflow, not the original driver's kernel ABI, multi-process precedence behavior or its historical unplug/ID failure mode.
