const {Interception,FilterKeyState,FilterMouseState}=require('interceptor-keyboard-mouse');
console.log('Monitor keyboard/mouse input and forward it unchanged. Press Escape to close.');
(async()=>{
  const input=new Interception();
  try {
    console.log(input.getDevices().map(device=>device.toString()));
    input.setFilter('keyboard',FilterKeyState.ALL);
    input.setFilter('mouse',FilterMouseState.ALL);
    for(;;){
      const device=await input.wait();if(!device)break;
      const stroke=device.receive();if(!stroke)continue;
      device.send(stroke); // Forward before slower console rendering.
      console.log(device.id,stroke);
      if(stroke.type==='keyboard'&&stroke.code===1)break;
    }
  }finally{if(!input.isDestroyed())input.destroy();}
})().catch(console.error);
