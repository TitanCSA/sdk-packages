# Interceptor-Gamepad-Controller JavaScript / TypeScript SDK

`npm install ./interceptor-gamepad-controller-1.0.1.tgz`

Run `InterceptorSetup.exe` from the [matching signed Interceptor Gamepad & Controller driver demo](https://github.com/TitanCSA/Interceptor-Gamepad-Controller/releases/tag/v1.0.13.0-demo) and restart Windows if setup requests it. The bundled 1.0.1 bridge names the correct installer when Windows reports a missing driver.

Node.js 20+ Windows x64. Bundled Node-API v8 addon and product native DLL; no compiler or Python required to install. Driver installation and original product licensing remain separate. Supported families: hid, xinput, xinputhid, gip.

```js
const { Interceptor } = require("interceptor-gamepad-controller");
const client = new Interceptor();
try { console.log(client.info()); } finally { client.close(); }
```

[API documentation](docs/API.md). Construction does not start capture.
