import java.util.Map;
import titancsa.interceptor.Interceptor;
class JavaTests {
 static void check(boolean condition){if(!condition)throw new AssertionError("Java bridge contract failed");}
 public static void main(String[]args){if(!"1".equals(System.getenv("INTERCEPTOR_TEST_BACKEND"))){System.out.println("SKIP: use Build.ps1 -Test");return;}try(var io=new Interceptor()){
  check(io.initialize().ready());check(io.devices().size()==6);check(io.devices().getFirst().name().contains("\u00e9"));
  var inputs=io.receive(0);check(inputs.size()==6&&inputs.getFirst().sequence==0x8000000000000001L);
  inputs.get(1).state.scalars.put("y",-7L);inputs.get(2).state.axes.getFirst().value=50;inputs.get(2).state.axes.getFirst().normalized=.5;inputs.get(5).blocked=true;
  check(io.send(inputs)==5);check(io.send(inputs)==0);check(io.inject("mouse",Map.of("x",200L))==1);check(!io.descriptor("fixture:4",true).applicable());
  var next=io.receive(0);try{io.send(inputs);throw new AssertionError("Stale batch accepted");}catch(IllegalArgumentException expected){}io.discard(next);
  System.out.println("PASS Java: FFM, all families, exact uint64 bits, Unicode, edits, HID, no replay, injection and stale tickets");
 }}
}
