# Interceptor JavaScript / TypeScript SDK

`npm install ./interceptor-sdk-1.0.1.tgz`

Run `InterceptorSetup.exe` from the [matching signed Interceptor driver demo](https://github.com/TitanCSA/Interceptor/releases/tag/v1.4.22.0-demo) and restart Windows if setup requests it. The bundled 1.0.1 bridge names the correct installer when Windows reports a missing driver.

Node.js 20+ Windows x64. Bundled Node-API v8 addon and product native DLL; no compiler or Python required to install. Driver installation and original product licensing remain separate. Supported families: keyboard, mouse, hid, xinput, xinputhid, gip.

```js
const { Interceptor } = require("interceptor-sdk");
const client = new Interceptor();
try { console.log(client.info()); } finally { client.close(); }
```

[API documentation](docs/API.md). Construction does not start capture.
