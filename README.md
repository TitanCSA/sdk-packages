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
| TitanCSA.RemoteWorkstation.Native | 0.1.1 |
| TitanCSA.StorageIOFlightRecorder.Native | 0.1.1 |
| Titan-Audio-Link.Native | 0.1.1 |
| Titan-File-Filter.Native | 0.1.0 |
| Titan-Network-Filter.Native | 0.1.1 |
| USB-over-IP-SDK.Native | 0.1.1 |
| USB-Trace-SDK.Native | 0.1.1 |
| TitanCSA.UserBackedVirtualDisk.Native | 0.1.1 |
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

<!-- centralized-sdk-catalog:start -->
## Catalogue des SDK

Un dossier par projet rassemble les sources SDK, recettes, licences, documentation et exemples. Les depots des produits gardent leur code produit ; les versions publiees restent immuables.

| Projet | Langages et commandes d installation |
| --- | --- |
| AutoHotInterceptor | [Guide et sources](projects/AutoHotInterceptor/README.md) |
| Browser-Control-SDK | [Guide et sources](projects/Browser-Control-SDK/README.md) |
| Changed-Block-Tracking-SDK | [Guide et sources](projects/Changed-Block-Tracking-SDK/README.md) |
| Cloud-Files-SDK | [Guide et sources](projects/Cloud-Files-SDK/README.md) |
| Connection-Trace-Studio | [Guide et sources](projects/Connection-Trace-Studio/README.md) |
| File-Trace-Studio | [Guide et sources](projects/File-Trace-Studio/README.md) |
| FyraDevice | [Guide et sources](projects/FyraDevice/README.md) |
| FyraInput | [Guide et sources](projects/FyraInput/README.md) |
| FyraInput-Gamepad | [Guide et sources](projects/FyraInput-Gamepad/README.md) |
| FyraInput-Keyboard-Mouse | [Guide et sources](projects/FyraInput-Keyboard-Mouse/README.md) |
| HID-Access-Control | [Guide et sources](projects/HID-Access-Control/README.md) |
| Interception-V2 | [Guide et sources](projects/Interception-V2/README.md) |
| Interceptor | [Guide et sources](projects/Interceptor/README.md) |
| Interceptor-Gamepad-Controller | [Guide et sources](projects/Interceptor-Gamepad-Controller/README.md) |
| Interceptor-Keyboard-Mouse | [Guide et sources](projects/Interceptor-Keyboard-Mouse/README.md) |
| Interceptor-MultiLang | [Guide et sources](projects/Interceptor-MultiLang/README.md) |
| Network-Emulation-SDK | [Guide et sources](projects/Network-Emulation-SDK/README.md) |
| node-interceptor | [Guide et sources](projects/node-interceptor/README.md) |
| Packet-Capture-SDK | [Guide et sources](projects/Packet-Capture-SDK/README.md) |
| Process-Trace-Studio | [Guide et sources](projects/Process-Trace-Studio/README.md) |
| ProcessGuard-SDK | [Guide et sources](projects/ProcessGuard-SDK/README.md) |
| RegistryGuard | [Guide et sources](projects/RegistryGuard/README.md) |
| Remote-Workstation-SDK | [Guide et sources](projects/Remote-Workstation-SDK/README.md) |
| Storage-IO-Flight-Recorder | [Guide et sources](projects/Storage-IO-Flight-Recorder/README.md) |
| Titan-Audio-Link | [Guide et sources](projects/Titan-Audio-Link/README.md) |
| Titan-File-Filter | [Guide et sources](projects/Titan-File-Filter/README.md) |
| Titan-Network-Filter | [Guide et sources](projects/Titan-Network-Filter/README.md) |
| USB-over-IP-SDK | [Guide et sources](projects/USB-over-IP-SDK/README.md) |
| USB-Trace-SDK | [Guide et sources](projects/USB-Trace-SDK/README.md) |
| User-Backed-Virtual-Disk-SDK | [Guide et sources](projects/User-Backed-Virtual-Disk-SDK/README.md) |
| Vdevice | [Guide et sources](projects/Vdevice/README.md) |
| Virtual-Camera-SDK | [Guide et sources](projects/Virtual-Camera-SDK/README.md) |
| Virtual-Network-Adapter-SDK | [Guide et sources](projects/Virtual-Network-Adapter-SDK/README.md) |
| Virtual-Storage-Fault-Lab | [Guide et sources](projects/Virtual-Storage-Fault-Lab/README.md) |
| Windows-Gamepad-Filter | [Guide et sources](projects/Windows-Gamepad-Filter/README.md) |
| Windows-Input-Filter | [Guide et sources](projects/Windows-Input-Filter/README.md) |
| Windows-Keyboard-Mouse-Filter | [Guide et sources](projects/Windows-Keyboard-Mouse-Filter/README.md) |
| Windows-Virtual-Devices | [Guide et sources](projects/Windows-Virtual-Devices/README.md) |

Les4 depots dont le proprietaire est suspendu restent exclus de cette migration. Les20 applications exclues sur demande ne sont pas modifiees.
<!-- centralized-sdk-catalog:end -->
