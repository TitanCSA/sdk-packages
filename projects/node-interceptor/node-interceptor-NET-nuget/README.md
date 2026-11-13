# node-interceptor .NET8 SDK

Package ID `NodeInterceptor.SDK`. ```csharp
using var client=new TitanCSA.Products.NodeInterceptor.Client();
string statusJson=client.Call("check", "{\"device\":1}");
```

Client implements IDisposable; Call takes raw argument JSON object and returns result JSON. NativeException exposes Code. UInt64 fields remain decimal strings. [API](docs/API.md). Windows x64 and separately installed matching driver required.
