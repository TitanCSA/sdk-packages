# FyraInput-Gamepad JavaScript / TypeScript SDK

`npm install fyrainput-gamepad`

Node.js 20+ Windows x64. Bundled Node-API v8 addon and product native DLL; no compiler or Python required to install. Driver installation and original product licensing remain separate. Supported families: hid, xinput, xinputhid, gip.

```js
const { Interceptor } = require("fyrainput-gamepad");
const client = new Interceptor();
try { console.log(client.info()); } finally { client.close(); }
```

[API documentation](docs/API.md). Construction does not start capture.
