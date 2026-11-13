'use strict';
const {test}=require('node:test');
const assert=require('node:assert/strict');
const path=require('node:path');
const {spawnSync}=require('node:child_process');
const addon=require(process.env.INTERCEPTOR_TEST_MODULE || path.resolve(__dirname,'../obj/Test/Release/node_interceptor.node'));
const loader=require.resolve('../tools/load.cjs');
require.cache[loader]={id:loader,filename:loader,loaded:true,exports:addon};
const {Interception,FilterKeyState,FilterMouseState,Device,installationStatus}=require('../dist/cjs/lib');
test('API compatibility, exact fields, cancellation, lifetime and validation',async()=>{
  assert.equal(installationStatus().installed,true);
  addon.testReset();const input=new Interception();
  try {
    assert.deepEqual(input.getDevices().map(d=>d.id),[1,2,11,12]);
    for(const id of [1,10])assert.equal(new Device(input.context,id).isKeyboard(),true);
    for(const id of [11,20])assert.equal(new Device(input.context,id).isMouse(),true);
    assert.throws(()=>new Interception(),/Another/);
    assert.throws(()=>addon.receive({},1),/Expected a context/);
    assert.throws(()=>input.waitSync(-1),/Timeout/);
    assert.throws(()=>input.setFilter('mouse',NaN),/16-bit/);
    assert.throws(()=>input.getDevices()[0].setPrecedence(10),/unavailable/);
    input.setFilter('keyboard',FilterKeyState.ALL);input.setFilter('mouse',FilterMouseState.ALL);
    assert.equal(input.waitSync(0),null);
    let pending=input.wait(1000);assert.throws(()=>input.waitSync(0),/Only one wait/);
    addon.testPushKey(102,30,1);let device=await pending;assert.equal(device.id,2);
    const stroke=device.receive();assert.deepEqual(stroke,{type:'keyboard',code:30,state:1,information:0});assert.equal(device.send(stroke),true);
    addon.testPushMouse(202,-485,300);device=await input.wait(1000);assert.equal(device.id,12);
    assert.deepEqual(device.receive(),{type:'mouse',state:0,flags:0,rolling:0,x:-485,y:300,information:0});
    assert.throws(()=>device.send({type:'keyboard',code:0,state:0}),/does not match/);
    assert.throws(()=>device.send({type:'mouse',state:0,flags:0,rolling:0,x:Infinity,y:0}),/out of range/);
    for(let n=0;n<2000;n++){addon.testPushKey(101,n,0);const d=await input.wait(1000);assert.equal(d.id,1);const k=d.receive();assert.equal(k.code,n);assert.equal(d.send(k),true);}
    pending=input.wait();input.destroy();assert.equal(await pending,null);
    assert.equal(input.isDestroyed(),true);assert.throws(()=>input.getDevices(),/destroyed/);
  } finally{if(!input.isDestroyed())input.destroy();}
  for(let n=0;n<100;n++){const i=new Interception();assert.equal(i.isDestroyed(),false);i.destroy();assert.equal(i.isDestroyed(),true);}
});
test('worker termination releases an outstanding native wait',()=>{
  const result=spawnSync(process.execPath,[path.join(__dirname,'worker-exit.cjs')],{encoding:'utf8',timeout:10000});
  assert.equal(result.status,0,result.stdout+result.stderr);
});
