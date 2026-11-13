const {Interception,FilterKeyState}=require('interceptor-keyboard-mouse');
console.log('Block only A on the first keyboard. Press Escape to close.');
(async()=>{
  const input=new Interception();
  try {
    const selected=input.getKeyboards()[0];if(!selected)throw Error('No keyboard found');
    input.setFilter('keyboard',FilterKeyState.ALL);
    for(;;){const device=await input.wait();if(!device)break;const stroke=device.receive();if(!stroke)continue;
      if(device.id!==selected.id||stroke.code!==0x1e)device.send(stroke);
      if(stroke.code===1)break;
    }
  }finally{if(!input.isDestroyed())input.destroy();}
})().catch(console.error);
