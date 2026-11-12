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

The `publish-nuget.yml` workflow verifies the hashes in `nuget-manifest.json`,
then exchanges its GitHub identity for a short-lived NuGet publishing credential.
The NuGet account must first authorize this repository, workflow and `nuget`
environment in Trusted Publishing. There are 2 managed .NET SDKs and 37 native
C/C++ source packages. Four unavailable upstream projects are excluded.

Native packages use `.Native` to distinguish them from managed assemblies.
Each was restored through NuGet and compiled as a Windows x64 Visual C++
consumer. API guides, usage instructions and license notices are bundled.

| Package | Version |
|---|---|
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

## Current registry status

- PyPI: browser-control-sdk 0.2.1 and interceptor-multilang 1.0.1 published; public pip installs verified.
- crates.io: interceptor-multilang 1.0.1 published; public Cargo consumer built and executed.
- Maven Central: io.github.titancsa:interceptor:1.0.1 published; public Maven consumer built and executed. Main JAR, sources and Javadoc hashes verified.
- NuGet: 39 packages built and tested; publication awaits owner account email confirmation and trusted publishing policy.
- Conda: two packages are downloadable from the custom conda-channel in this repository and public consumer installation is verified.
- Official conda-forge catalogue: recipes submitted in conda-forge/staged-recipes#35612 and #35613, pending CI completion and maintainer review.
