const {Worker,isMainThread,parentPort,workerData}=require('node:worker_threads');
const path=require('node:path');
if(isMainThread){
  const worker=new Worker(__filename,{workerData:{addon:process.env.INTERCEPTOR_TEST_MODULE || path.resolve(__dirname,'../obj/Test/Release/node_interceptor.node')}});
  const timeout=setTimeout(()=>{console.error('Worker termination did not release capture');process.exit(3);},5000);
  worker.once('message',async()=>{await worker.terminate();clearTimeout(timeout);console.log('Worker termination cancelled native wait');});
}else{
  const addon=require(workerData.addon);const c=addon.createContext();addon.waitAsync(c);parentPort.postMessage('waiting');
}
