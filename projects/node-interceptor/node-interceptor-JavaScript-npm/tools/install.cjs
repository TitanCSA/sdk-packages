'use strict';
// Package installation never elevates privileges or modifies drivers.
require('./load.cjs');
console.log('Node addon ready. Install the driver once: npx interceptor-keyboard-mouse /install');
