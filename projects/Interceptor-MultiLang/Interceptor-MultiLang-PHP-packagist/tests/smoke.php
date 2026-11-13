<?php
require dirname(__DIR__).'/src/Interceptor.php';
use TitanCSA\Interceptor\Interceptor;
function verify($condition,$message) { if (!$condition) throw new RuntimeException($message); }
$io=new Interceptor();
try {
 if (getenv('INTERCEPTOR_TEST_BACKEND')==='1') {
  verify($io->initialize()['ready']===1,'initialize');
  $devices=$io->devices();verify(count($devices)===6,'six devices');verify(str_contains($devices[0]['name'],"\u{00e9}"),'UTF-8');
  $batch=$io->receive(0);verify(count($batch)===6,'six kinds');verify($batch->events[0]->sequence==='9223372036854775809','uint64');
  $batch->events[0]->field('scan_code')->value=0x30;$batch->events[1]->field('y')->value*=-1;
  $axes=array_values(array_filter($batch->events[2]->fields,fn($f)=>$f->metadata['group']===1));verify(count($axes)===2,'repeated HID axes');
  $axes[0]->value=50;$axes[0]->normalized=.5;
  $batch->events[5]->blocked=true;
  verify($io->send($batch)===5,'send/block');verify($io->send($batch)===0,'idempotent retry');
  verify($io->inject('mouse',['x'=>200])===1,'inject');
  $newer=$io->receive(0);$rejected=false;try{$io->send($batch);}catch(InvalidArgumentException){$rejected=true;}verify($rejected,'stale batch');
  $io->discard($newer);$io->cancel();
  $io->close(); $other=new Interceptor();try{$rejected=false;try{$other->send($newer);}catch(InvalidArgumentException){$rejected=true;}verify($rejected,'foreign batch');}finally{$other->close();}
  try{$io->receive(-2);throw new RuntimeException('negative timeout accepted');}catch(InvalidArgumentException){}
  echo "PHP native fixture round-trip passed\n";
 } else { echo "PHP bundled production native DLL lazy open/close passed\n"; }
} finally { $io->close(); }
