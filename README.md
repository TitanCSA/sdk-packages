# TitanCSA SDK package distribution

Reviewed user-mode SDK distributions and registry publishing workflows.
No registry credentials are stored in this repository. PyPI publishing uses
GitHub OIDC with one workflow per pending PyPI project.

| Package | Version | Install | Workflow |
|---|---|---|---|
| browser-control-sdk | 0.2.1 | `python -m pip install browser-control-sdk` | publish-pypi.yml |
| interceptor-multilang | 1.0.1 | `python -m pip install interceptor-multilang` | publish-interceptor-pypi.yml |

Commands become available after a successful registry publication. Workflow logs
and the registry itself determine publication status; committing an archive does
not publish it to PyPI.

Each distribution includes its README, API guide, usage example and licenses.
Interceptor MultiLang requires Windows x64 and a matching separately installed
driver; Browser Control requires the separately installed BrowserControl app.
Installing these packages does not install a kernel driver.

Sources: [Browser Control](https://github.com/TitanCSA/Browser-Control-SDK),
[Interceptor MultiLang](https://github.com/TitanCSA/Interceptor-MultiLang).
Artifact hashes are recorded in pypi-manifest.json and checked before upload.
Licenses are specific to each package and included inside its distributions.

## NuGet distributions

76 reviewed packages:38 managed SDKs and38 native source SDKs. Four unavailable upstream repositories remain excluded. The workflow verifies every archive hash before requesting a short-lived publishing key. It skips only versions whose public content matches the reviewed package and never replaces an existing version. Source licenses and driver/application prerequisites are retained. Native source packages are compiled by the consuming Visual C++ project.

Configure NuGet Trusted Publishing for repository owner `TitanCSA`, repository `sdk-packages`, workflow `publish-nuget.yml`, environment `nuget`; enable new packages and new versions for the intended package IDs. The workflow uses the policy creator profile name, not an email.

| Package | Version |
| --- | --- |
| Interceptor.MultiLang | 1.0.1 |
| AutoHotInterceptor | 1.0.1 |
| AutoHotInterceptor.Native | 1.0.0 |
| Browser-Control-SDK.Native | 0.2.0 |
| Changed-Block-Tracking-SDK.Native | 0.1.1 |
| Cloud-Files-SDK.Native | 0.1.1 |
| Connection-Trace-Studio.Native | 0.1.0 |
| File-Trace-Studio.Native | 0.1.0 |
| FyraDevice.Native | 0.1.0 |
| FyraInput.Native | 0.1.0 |
| FyraInput-Gamepad.Native | 0.1.0 |
| FyraInput-Keyboard-Mouse.Native | 0.1.0 |
| HID-Access-Control.Native | 0.1.0 |
| Interception-V2.Native | 2.0.0 |
| Interceptor.Native | 1.4.22 |
| Interceptor-Gamepad-Controller.Native | 1.0.13 |
| Interceptor-Keyboard-Mouse.Native | 1.0.5 |
| Interceptor-MultiLang.Native | 1.0.0 |
| Network-Emulation-SDK.Native | 0.1.1 |
| Packet-Capture-SDK.Native | 0.1.1 |
| Process-Trace-Studio.Native | 0.1.0 |
| ProcessGuard-SDK.Native | 0.1.2 |
| RegistryGuard.Native | 0.1.1 |
| Remote-Workstation-SDK.Native | 0.1.1 |
| Storage-IO-Flight-Recorder.Native | 0.1.1 |
| Titan-Audio-Link.Native | 0.1.1 |
| Titan-File-Filter.Native | 0.1.0 |
| Titan-Network-Filter.Native | 0.1.1 |
| USB-over-IP-SDK.Native | 0.1.1 |
| USB-Trace-SDK.Native | 0.1.1 |
| User-Backed-Virtual-Disk-SDK.Native | 0.1.1 |
| Vdevice.Native | 1.0.5 |
| Virtual-Camera-SDK.Native | 0.1.1 |
| Virtual-Network-Adapter-SDK.Native | 0.1.1 |
| Virtual-Storage-Fault-Lab.Native | 0.1.1 |
| Windows-Gamepad-Filter.Native | 0.1.0 |
| Windows-Input-Filter.Native | 0.1.0 |
| Windows-Keyboard-Mouse-Filter.Native | 0.1.0 |
| Windows-Virtual-Devices.Native | 0.1.0 |
| FyraInput.SDK | 1.0.0 |
| FyraInputGamepad.SDK | 1.0.0 |
| FyraInputKeyboardMouse.SDK | 1.0.0 |
| Interceptor.SDK | 1.0.0 |
| InterceptorGamepadController.SDK | 1.0.0 |
| InterceptorKeyboardMouse.SDK | 1.0.0 |
| WindowsGamepadFilter.SDK | 1.0.0 |
| WindowsInputFilter.SDK | 1.0.0 |
| WindowsKeyboardMouseFilter.SDK | 1.0.0 |
| FyraDevice.SDK | 1.0.0 |
| Vdevice.SDK | 1.0.0 |
| WindowsVirtualDevices.SDK | 1.0.0 |
| FileTraceStudio.SDK | 1.0.0 |
| ConnectionTraceStudio.SDK | 1.0.0 |
| ProcessTraceStudio.SDK | 1.0.0 |
| ProcessGuardSDK.SDK | 1.0.0 |
| RegistryGuard.SDK | 1.0.0 |
| StorageIOFlightRecorder.SDK | 1.0.0 |
| VirtualStorageFaultLab.SDK | 1.0.0 |
| TitanFileFilter.SDK | 1.0.0 |
| TitanNetworkFilter.SDK | 1.0.0 |
| NetworkEmulationSDK.SDK | 1.0.0 |
| PacketCaptureSDK.SDK | 1.0.0 |
| USBTraceSDK.SDK | 1.0.0 |
| VirtualNetworkAdapterSDK.SDK | 1.0.0 |
| VirtualCameraSDK.SDK | 1.0.0 |
| ChangedBlockTrackingSDK.SDK | 1.0.0 |
| CloudFilesSDK.SDK | 1.0.0 |
| HIDAccessControl.SDK | 1.0.0 |
| USBoverIPSDK.SDK | 1.0.0 |
| UserBackedVirtualDiskSDK.SDK | 1.0.0 |
| RemoteWorkstationSDK.SDK | 1.0.0 |
| TitanAudioLink.SDK | 1.0.0 |
| InterceptionV2.SDK | 1.0.0 |
| NodeInterceptor.SDK | 1.0.0 |
| BrowserControlSDK.SDK | 1.0.0 |
| NodeInterceptor.Native | 1.0.0 |
