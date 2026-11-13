using TitanCSA.Interceptor;
if(Environment.GetEnvironmentVariable("INTERCEPTOR_TEST_BACKEND")!="1"){Console.WriteLine("SKIP: use Build.ps1 -Test");return;}
using var io=new Interceptor();
void Check(bool value){if(!value)throw new Exception(".NET bridge contract failed");}
Check(io.Initialize().Ready);Check(io.Devices().Count==6);Check(io.Devices()[0].Name.Contains('\u00e9'));
var inputs=io.Receive(0);Check(inputs.Count==6&&inputs[0].Sequence==0x8000000000000001UL);
inputs[1].State["y"]=-7;inputs[2].State.Axes[0].Value=50;inputs[2].State.Axes[0].Normalized=.5;inputs[5].Blocked=true;
Check(io.Send(inputs)==5);Check(io.Send(inputs)==0);Check(io.Inject("mouse",new Dictionary<string,long>{{"x",200}})==1);
Check(!io.GetDescriptor("fixture:4",true).Applicable);
var next=io.Receive(0);try{io.Send(inputs);throw new Exception("Stale batch accepted");}catch(ArgumentException){}io.Discard(next);
Console.WriteLine("PASS .NET: all families, Unicode, uint64, edits, HID, block, no replay, descriptors, injection and stale tickets");
