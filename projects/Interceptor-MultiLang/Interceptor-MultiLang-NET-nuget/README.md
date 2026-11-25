# Interceptor.MultiLang

Windows x64 .NET 8+ wrapper for the combined Interceptor keyboard, mouse and gamepad driver.

## Install the driver and the .NET package

Use the [Microsoft-signed combined Interceptor demo 1.4.22.0](https://github.com/TitanCSA/Interceptor/releases/tag/v1.4.22.0-demo). Run InterceptorSetup.exe and restart Windows if the installer requests it. The [complete MultiLang ZIP](https://github.com/TitanCSA/Interceptor-MultiLang/releases/latest) also supplies Install.cmd.

    dotnet add package Interceptor.MultiLang --version 1.0.2

The NuGet package includes the Windows x64 native bridge. It reports the matching installer when Windows error 2 indicates a missing driver.

## Read driver status

    using TitanCSA.Interceptor;

    using var io = new Interceptor();
    Console.WriteLine(io.GetInfo());

The status query leaves input capture off. When implementing capture, forward every input family or set Blocked to deliberately suppress entries. Dispose the client to restore passthrough. See the [API guide and examples](https://github.com/TitanCSA/Interceptor-MultiLang/blob/main/docs/API.md).

The one-hour demo allowance begins after the first documented capture or controller-monitor operation. See the [edition comparison](https://titancsa.github.io/guides/windows-input-release-compare/) for exact limits and the commercial Release.

The wrapper is MIT licensed. The signed Windows driver is distributed separately under its binary demo terms.