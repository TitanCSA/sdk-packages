const {spawnSync}=require('node:child_process');
const path=require('node:path');
for(let n=0;n<100;n++){
  const result=spawnSync(process.execPath,[path.join(__dirname,'worker-exit.cjs')],{encoding:'utf8',timeout:10000});
  if(result.status!==0){console.error({iteration:n,status:result.status,error:result.stderr});process.exit(1);}
}
console.log('100 worker lifecycles passed',process.version);
