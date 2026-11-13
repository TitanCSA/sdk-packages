<?php
declare(strict_types=1);
namespace TitanCSA\Products\VirtualStorageFaultLab;
use FFI;
final class NativeException extends \RuntimeException {}
final class Client {
 private FFI $ffi;
 private int $session=0;
 public function __construct(?string $libraryPath=null){
  if(PHP_OS_FAMILY!=='Windows' || PHP_INT_SIZE!==8)throw new \RuntimeException('Windows x64 PHP required');
  $this->ffi=FFI::cdef('unsigned int virtual_storage_fault_lab_json_open(uint64_t*);unsigned int virtual_storage_fault_lab_json_close(uint64_t);char* virtual_storage_fault_lab_json_call(uint64_t,const char*,uint32_t);void virtual_storage_fault_lab_json_free(char*);',$libraryPath??(getenv('VIRTUAL_STORAGE_FAULT_LAB_SDK_NATIVE_PATH')?:dirname(__DIR__).'/native/VirtualStorageFaultLabJsonSDK.dll'));
  $id=$this->ffi->new('uint64_t');$code=$this->ffi->virtual_storage_fault_lab_json_open(FFI::addr($id));if($code)throw new NativeException('Cannot open SDK',$code);$this->session=$id->cdata;
 }
 public function call(string $operation,array|object $arguments=[]):mixed{
  if(!$this->session)throw new \LogicException('SDK is closed');
  $payload=json_encode(['operation'=>$operation,'arguments'=>$arguments===[]?(object)[]:$arguments],JSON_THROW_ON_ERROR|JSON_UNESCAPED_UNICODE);
  if(strlen($payload)>32*1024*1024)throw new \InvalidArgumentException('Request exceeds 32 MiB');
  $pointer=$this->ffi->virtual_storage_fault_lab_json_call($this->session,$payload,strlen($payload));if(FFI::isNull($pointer))throw new \RuntimeException('Native response allocation failed');
  try{$response=json_decode(FFI::string($pointer),true,512,JSON_THROW_ON_ERROR);}finally{$this->ffi->virtual_storage_fault_lab_json_free($pointer);}
  if(!$response['ok'])throw new NativeException($response['message'],$response['code']);return $response['result'];
 }
 public function close():void{if($this->session){$code=$this->ffi->virtual_storage_fault_lab_json_close($this->session);if($code)throw new NativeException('Native close failed',$code);$this->session=0;}}
 public function __destruct(){try{$this->close();}catch(\Throwable){}}
}
