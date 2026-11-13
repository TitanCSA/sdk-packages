const {Interception,FilterKeyState}=require('interceptor-keyboard-mouse');
console.log('Remap A to B on all keyboards. Press Escape to close.');
(async()=>{
  const input=new Interception();
  try {
    input.setFilter('keyboard',FilterKeyState.ALL);
    for(;;){const device=await input.wait();if(!device)break;const stroke=device.receive();if(!stroke)continue;
      if(stroke.code===0x1e)stroke.code=0x30;
      device.send(stroke);
      if(stroke.code===1)break;
    }
  }finally{if(!input.isDestroyed())input.destroy();}
})().catch(console.error);
