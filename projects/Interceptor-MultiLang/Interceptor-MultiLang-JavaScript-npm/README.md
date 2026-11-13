# Interceptor MultiLang for JavaScript and TypeScript

Node.js bindings for the combined Interceptor Windows input driver.
Requires Windows x64, Node.js 20+, and the matching full Interceptor 1.4.22.0 driver.
The native Node addon and bridge DLL are bundled. Installing this npm package
does not install a kernel driver.

```sh
npm install interceptor-multilang
```

Load the API without starting capture:

```javascript
const { Interceptor } = require('interceptor-multilang');
const io = new Interceptor();
try {
  console.log(io.info());
} finally {
  io.close();
}
```

For capture, install the matching driver separately and use an administrator
terminal. Split Keyboard/Mouse editions and Interception V2 use different interfaces.
Always forward the complete received batch, or mark deliberately blocked events.
Closing the client releases capture; physical Ctrl+Alt+Delete is the driver recovery shortcut.

- [Complete API reference and event fields](docs/API.md)
- [Project guide, supported devices and driver setup](docs/PROJECT.md)
- [Usage example](examples/info.cjs)
- [Matching driver and complete release](https://github.com/TitanCSA/Interceptor-MultiLang/releases)

TypeScript declarations and documentation are included in the package.
The driver's demo restrictions and licensing remain separate from this MIT wrapper.
License: [LICENSE](LICENSE).
