<?php
require dirname(__DIR__).'/src/Interceptor.php';
use TitanCSA\Products\WindowsKeyboardMouseFilter\Interceptor;
use TitanCSA\Products\WindowsKeyboardMouseFilter\NativeException;
function check($v){if(!$v)throw new RuntimeException('Product PHP contract failed');}
$fixture=$argv[1]??null;
$io=new Interceptor($fixture);
try {
 if($fixture){
  $kinds=["keyboard", "mouse"];$mode=FFI::cdef('void windows_keyboard_mouse_filter_test_mode(unsigned int);',$fixture);
  check($io->initialize()['ready']===1);
  check($io->info()['input_mask']===3 && $io->info()['controller_mask']===0);
  check(count($io->devices())===count($kinds));
  $batch=$io->receive(0);check(count($batch)===count($kinds));
  foreach($batch as $event){check(strlen($event->sequence)===19 && strcmp($event->sequence,'9223372036854775807')>0);if($event->kind==='keyboard')$event->field('scan_code')->value=48;if($event->kind==='mouse')$event->field('y')->value=-7;}
  check($io->send($batch)===count($kinds));check($io->send($batch)===0);
  $new=$io->receive(0);try{$io->send($batch);throw new RuntimeException('stale accepted');}catch(InvalidArgumentException){}
  $new->events[0]->blocked=true;check($io->send($new)===count($kinds)-1);
  $retry=$io->receive(0);$mode->windows_keyboard_mouse_filter_test_mode(2);
  try{$io->send($retry);throw new RuntimeException('partial missing');}catch(NativeException $e){check($e->getCode()===170 && $e->accepted===1);}
  check($io->send($retry)===count($kinds)-1);$mode->windows_keyboard_mouse_filter_test_mode(0);
  $io->discard($io->receive(0));$io->cancel();
  foreach(array_diff(['keyboard','mouse','hid','xinput','xinputhid','gip'],$kinds) as $kind){try{$io->inject($kind,[]);throw new RuntimeException('unsupported accepted');}catch(NativeException $e){check($e->getCode()===50);}}
 }
}finally{$io->close();}
echo "Product PHP contract and lazy native load passed\n";
