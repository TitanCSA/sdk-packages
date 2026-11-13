# Build and test

Use Node.js 20+, Visual Studio 2022 C++ tools, Python and a Windows SDK:

```powershell
npm ci --ignore-scripts
npm run build
npm run build:addon
```

The addon uses Node-API 8, avoiding a V8-version-specific binding. `binding.gyp` includes the matching Node host delay-load mechanism. CMake is also supported with `NODE_HEADERS` and `NODE_LIBRARY` pointing to matching Node headers and Windows `node.lib`; choose x64 or Win32 consistently.

For deterministic tests, configure CMake with `-DINTERCEPTOR_TEST_BACKEND=ON`, `-DNODE_HEADERS=...`, `-DNODE_LIBRARY=...` into **obj/Test**, then build Release and run `npm test`. Test source uses the synthetic SDK in `test/native`; it never accesses physical input. Production binaries must be built with the test backend **off**. The test-only `testPushKey`/`testPushMouse` exports must never appear in release addons.

The source distribution includes the addon, JS/TypeScript wrapper and Interceptor SDK. Static MSVC runtime linkage removes the need for a separate VC runtime installer. Only the existing signed driver installers are redistributed; kernel source is excluded.

Use the release `.tgz` for deployment without compiler tools. To produce it from source, build both architectures into `prebuilds/win32-x64` and `prebuilds/win32-ia32`, compile TypeScript, then `npm pack`. `npm install` checks that a prebuilt addon exists and never requests elevation. `/install`, `/uninstall` and `--ui` explicitly request driver setup through UAC.
