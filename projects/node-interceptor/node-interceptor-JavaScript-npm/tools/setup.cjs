#!/usr/bin/env node
'use strict';
const { spawnSync } = require('node:child_process');
const path = require('node:path');
const fs = require('node:fs');
const command = process.argv[2];
if (!['/install', '/uninstall', '--status', '--ui'].includes(command)) {
  console.log('Usage: interceptor-keyboard-mouse /install | /uninstall | --ui | --status\nDriver setup requests administrator approval; restart Windows when requested.');
  process.exitCode = command ? 2 : 0;
} else if (command === '--status') {
  console.log(JSON.stringify(require('./load.cjs').installationStatus(), null, 2));
} else {
  const exe = path.resolve(__dirname, '../driver', command === '--ui' ? 'InterceptorSetup.exe' : 'InterceptorSetupConsole.exe');
  if (!fs.existsSync(exe)) throw new Error('Driver installer missing; extract/install the complete release package.');
  // Fixed installer path in an environment variable, never concatenate user input
  // into PowerShell. Start-Process RunAs handles elevation and waits for completion.
  const script = "$p=Start-Process -FilePath $env:TITANCSA_SETUP_EXE -ArgumentList $env:TITANCSA_SETUP_ARGUMENT -Verb RunAs -Wait -PassThru; exit $p.ExitCode";
  const uiScript = "$p=Start-Process -FilePath $env:TITANCSA_SETUP_EXE -Verb RunAs -Wait -PassThru; exit $p.ExitCode";
  const result = spawnSync('powershell.exe', ['-NoProfile', '-NonInteractive', '-Command', command === '--ui' ? uiScript : script], {
    stdio:'inherit', windowsHide:true, shell:false,
    env:{...process.env,TITANCSA_SETUP_EXE:exe,TITANCSA_SETUP_ARGUMENT:command}
  });
  if (result.error) throw result.error;
  if (result.status === 3010) console.log('Installation succeeded. Restart Windows before capturing input.');
  else if (result.status !== 0) console.error(`Setup failed (exit ${result.status}). Review the driver installer log.`);
  process.exitCode = result.status ?? 1;
}
