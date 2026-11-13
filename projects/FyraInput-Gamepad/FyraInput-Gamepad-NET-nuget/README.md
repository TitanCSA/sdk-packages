# FyraInput-Gamepad .NET SDK

.NET 8+ Windows x64. Package ID: `FyraInputGamepad.SDK`; Installation: `dotnet add package FyraInputGamepad.SDK --version 1.0.0`.

```csharp
using TitanCSA.Products.FyraInputGamepad;
using var client = new Interceptor();
var info = client.GetInfo();
```

Native DLL bundled as runtime asset. No driver installation or capture on construction. [API](docs/API.md). Original licensing/trial prerequisites remain unchanged.
