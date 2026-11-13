const { Interceptor } = require('../../bindings/node');

async function main() {
  console.log('Invert mouse Y. Press Ctrl+C to restore normal input.');
  const io = new Interceptor();
  process.once('SIGINT', () => io.close());
  try {
    for (;;) {
      const inputs = await io.receive();
      for (const input of inputs)
        if (input.kind === 'mouse' && !(input.state.flags & 1)) input.state.y = -input.state.y;
      io.send(inputs);
    }
  } finally { io.close(); }
}
main().catch(error => { console.error(error.message); process.exitCode = 1; });
