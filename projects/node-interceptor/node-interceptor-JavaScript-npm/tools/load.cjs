'use strict';
const fs = require('node:fs');
const path = require('node:path');
if (process.platform !== 'win32' || !['x64', 'ia32'].includes(process.arch))
  throw new Error('node-interceptor requires Windows x64 (x64 or ia32 Node.js).');
const root = path.resolve(__dirname, '..');
const candidates = [path.join(root, 'prebuilds', `win32-${process.arch}`, 'node_interceptor.node'), path.join(root, 'build', 'Release', 'node_interceptor.node')];
const addon = candidates.find(file => fs.existsSync(file));
if (!addon) throw new Error('Native addon missing. Use the release .tgz, or run npm run build:addon with Visual Studio C++ tools installed.');
module.exports = require(addon);
