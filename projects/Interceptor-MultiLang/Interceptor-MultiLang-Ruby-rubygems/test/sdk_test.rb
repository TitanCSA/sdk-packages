require_relative '../lib/interceptor_multilang'
def verify(condition, message); raise message unless condition; end
io=InterceptorMultiLang::Interceptor.new
begin
 if ENV['INTERCEPTOR_TEST_BACKEND']=='1'
  verify(io.initialize_devices[:ready]==1,'initialize');devices=io.devices;verify(devices.size==6,'devices');verify(devices[0][:name].include?("\u00e9"),'UTF-8')
  batch=io.receive(timeout:0);verify(batch.size==6,'six event kinds');verify(batch[0].sequence==0x8000000000000001,'uint64')
  batch[0].field('scan_code').value=0x30;batch[1].field('y').value *= -1
  axes=batch[2].fields.select{|f|f.metadata[:group]==1};verify(axes.size==2,'repeated HID fields');axes[0].value=50;axes[0].normalized=0.5
  batch[5].blocked=true;verify(io.send(batch)==5,'send/block');verify(io.send(batch)==0,'retry');verify(io.inject('mouse',{'x'=>200})==1,'inject')
  newer=io.receive(timeout:0);begin;io.send(batch);raise 'stale accepted';rescue ArgumentError;end;io.discard(newer)
  api=Fiddle.dlopen(ENV.fetch('INTERCEPTOR_NATIVE_PATH'));mode=Fiddle::Function.new(api['icb_test_mode'],[Fiddle::TYPE_INT],Fiddle::TYPE_VOID)
  mode.call(1);result=nil;t=Thread.new {result=io.receive};sleep 0.05;io.cancel;t.join(2);verify(!t.alive? && result.size==0,'cancellation');mode.call(0)
  io.close;other=InterceptorMultiLang::Interceptor.new
  begin;begin;other.send(newer);raise 'foreign accepted';rescue ArgumentError;end;ensure;other.close;end
  puts 'Ruby fixture passed: six families, UTF-8, uint64, edits, HID, send/block, retry, injection, stale/foreign and threaded cancellation'
 else
  puts 'Ruby bundled production native DLL lazy open/close passed'
 end
ensure
 io.close
end
