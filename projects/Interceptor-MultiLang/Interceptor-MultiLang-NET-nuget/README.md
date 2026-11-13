# Interceptor.MultiLang

Windows x64 .NET 8+ wrapper for the Interceptor keyboard, mouse and gamepad driver.

Install the Microsoft-signed driver from the [complete release](https://github.com/TitanCSA/Interceptor-MultiLang/releases/latest), then run your capture application as administrator.

```csharp
using TitanCSA.Interceptor;
using var io = new Interceptor();
while (true) {
    var inputs = io.Receive();
    foreach (var input in inputs)
        if (input.Kind == "mouse" && (input.State["flags"] & 1) == 0) input.State["y"] = -input.State["y"];
    io.Send(inputs);
}
```

Receiving consumes input. Forward every family, or set `Blocked` to deliberately suppress an entry. Dispose the client to restore passthrough. The existing one-hour-per-boot driver demo applies; contact **titancsa** on Discord for unrestricted licensing.

The native wrapper is MIT licensed and included in this package. The kernel driver is distributed separately under its binary demo terms; its source is private.

## Install from NuGet

```sh
dotnet add package Interceptor.MultiLang --version 1.0.1
```

[Source and API](https://github.com/TitanCSA/Interceptor-MultiLang)
