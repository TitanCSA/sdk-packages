const test = require('node:test');
const assert = require('node:assert/strict');
const { Interceptor } = require('../bindings/node');
test('all families, edits, exact uint64, injection, no replay and stale batches', { skip: process.env.INTERCEPTOR_TEST_BACKEND !== '1' }, async () => {
  const io = new Interceptor();
  try {
    assert.equal(io.initialize().ready, true);
    assert.equal(io.devices().length, 6);
    assert.match(io.devices()[0].name, /\u00e9/);
    const inputs = await io.receive(0);
    assert.equal(inputs.length, 6);
    assert.equal(inputs[0].sequence, 0x8000000000000001n);
    inputs[1].state.y *= -1;
    assert.equal(inputs[2].state.axes.length, 2);
    inputs[2].state.axes[0].value = 50;
    inputs[2].state.axes[0].normalized = .5;
    inputs[5].blocked = true;
    assert.equal(io.send(inputs), 5);
    assert.equal(io.send(inputs), 0);
    assert.equal(io.inject({kind:'mouse', state:{x:200}}), 1);
    const next = await io.receive(0);
    assert.throws(() => io.send(inputs), /Stale/);
    io.discard(next);
    assert.equal(io.descriptor('fixture:4', true).applicable, false);
  } finally { io.close(); }
});
test('wait cancellation leaves the JS event loop responsive; close is safe', { skip: process.env.INTERCEPTOR_TEST_BACKEND !== '1' }, async () => {
  const backend=require(process.env.INTERCEPTOR_NODE_PATH);
  const io=new Interceptor();backend._fixtureMode(1);
  try {
    const pending=io.receive();
    await new Promise(resolve=>setTimeout(resolve,20));
    assert.throws(()=>io.devices(), /busy/);
    io.cancel();assert.equal((await pending).length,0);
    const closing=io.receive();
    io.close();assert.equal((await closing).length,0);
  } finally { backend._fixtureMode(0);io.close(); }
});
