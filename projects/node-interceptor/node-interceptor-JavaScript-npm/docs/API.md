# JavaScript / TypeScript API

```javascript
const api = require('interceptor-keyboard-mouse');
const input = new api.Interception();
```

Construction initializes this installed driver and starts capture/demo use. Failures throw English errors; missing installation is not a valid empty context. `installationStatus()` is a read-only `{installed, error, message}` query and does not create a context or start the demo.

This status probes access to the driver API. `installed=false` with Windows error **5** means this process lacks access; run it as administrator before concluding that installation is missing. Error **2** means the control endpoint was not found.

## Interception

| Method | Return |
| --- | --- |
| `getDevices()` | Connected `Device[]` |
| `getDevice(id)` | Connected device or `null` |
| `getKeyboards()` | Keyboard devices |
| `getMice()` | Mouse devices (original API spelling retained) |
| `setFilter('keyboard', FilterKeyState)` | Sets this family's selected event flags |
| `setFilter('mouse', FilterMouseState)` | Sets this family's selected event flags |
| `wait(timeout?)` | `Promise<Device \| null>`; omitted timeout waits indefinitely |
| `waitSync(timeout?)` | `Device \| null`; blocks the calling JS thread |
| `isDestroyed()` | `boolean` |
| `destroy()` | Cancels waits, releases capture; repeated wrapper calls throw |
| `context` | Opaque context object, rejected after destruction |
| `toString()` | Descriptive string |

Timeout is an integer number of milliseconds, 0–4294967294. `0` polls. Only one wait may be pending per context. Timeout resolves/returns `null`; a native fault rejects/throws an `Error` with `win32Code`. Explicit destruction resolves a pending asynchronous wait to `null`.

## Device

| Member | Meaning |
| --- | --- |
| `id` | Capture-local ID: keyboard 1–10; mouse 11–20 |
| `getHardwareId()` | Hardware-ID string or `null`; duplicate models may match |
| `getFilter()` | Current family filter for this ID |
| `receive()` | Consumes one selected stroke or returns `null` |
| `send(stroke)` | Sends one stroke; `true` when accepted, throws on native failure |
| `isKeyboard()`, `isMouse()`, `isInvalid()` | Type guards based on ID ranges |
| `getPrecedence()` | `0`, the only supported single-owner priority |
| `setPrecedence(0)` | Accepts default priority; other values explicitly unsupported |
| `context`, `toString()` | Owning opaque context / descriptive text |

`Mouse` and `Keyboard` are TypeScript aliases of `Device<MouseStroke>` / `Device<KeyboardStroke>`. `getHardwareId()` identifies the model/stack shape; it is not a secure unique serial or durable capture token.

## Stroke layouts

```typescript
type KeyboardStroke = {
  type: 'keyboard';
  code: number;         // unsigned 16-bit scan code
  state: KeyState;      // DOWN=0, UP=1, E0=2, E1=4; flags may combine
  information: number;  // unsigned 32-bit extra information
};
type MouseStroke = {
  type: 'mouse';
  state: MouseState;    // button transitions and wheel flags
  flags: MouseFlag;     // relative/absolute, virtual desktop, no-coalesce...
  rolling: number;     // signed 16-bit wheel data, preserved exactly
  x: number;           // signed 32-bit relative delta or absolute coordinate
  y: number;
  information: number;
};
```

Destination ID and `type` must agree. Numeric fields must be finite integers in range. `information` may be omitted when synthesizing, defaulting to zero; received strokes always include it. Sending does not consume the JS object, so deliberate repeated injection is possible. It does not create virtual devices.

`FilterKeyState`: `NONE`, `ALL`, `DOWN`, `UP`, `E0`, `E1`, `TERMSRV_SET_LED`, `TERMSRV_SHADOW`, `TERMSRV_VKPACKET`.

`FilterMouseState`: `NONE`, `ALL`, `MOVE`, five `BUTTON_n_DOWN` / `BUTTON_n_UP` pairs, `WHEEL`, `H_WHEEL`, plus `LEFT_`, `RIGHT_`, `MIDDLE_` aliases. Combine flags using `|`. An event matching any selected component is delivered whole, so preserve its other fields when editing it.

`MouseState` contains the same transition/wheel bits; `MouseFlag` contains `MOVE_RELATIVE=0`, `MOVE_ABSOLUTE=1`, `VIRTUAL_DESKTOP=2`, `ATTRIBUTES_CHANGED=4`, `MOVE_NOCOALESCE=8`, `TERMSRV_SRC_SHADOW=256`.

`KeyState` exposes the corresponding keyboard flags. `MAX_KEYBOARD=10`, `MAX_MOUSE=10`, `MAX_DEVICE=20` are exported. The original low-level native wrapper remains internal; application code should use `Interception` and `Device`.

## Errors and lifecycle

Always destroy in `finally`, including after filter/callback/send errors. The driver releases input if the process ends. A capture revoked by another program, Ctrl+Alt+Delete, expiry or queue overflow must be closed and recreated after resolving the cause; successful timeout polling does not reactivate it. Runtime/validation errors are English; operating-system diagnostics retain numeric Windows codes.
