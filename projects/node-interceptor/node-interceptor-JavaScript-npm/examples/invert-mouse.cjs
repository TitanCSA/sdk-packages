const {Interception,FilterMouseState}=require('interceptor-keyboard-mouse');
console.log('Invert relative vertical mouse movement. Ctrl+C closes this console script.');
(async()=>{
  const input=new Interception();
  process.once('SIGINT',()=>{if(!input.isDestroyed())input.destroy();});
  try {
    input.setFilter('mouse',FilterMouseState.ALL);
    while(!input.isDestroyed()){const device=await input.wait();if(!device)break;const stroke=device.receive();if(!stroke)continue;
      if(!(stroke.flags&1))stroke.y=stroke.y===-2147483648?2147483647:-stroke.y;
      device.send(stroke);
    }
  }finally{if(!input.isDestroyed())input.destroy();}
})().catch(console.error);
