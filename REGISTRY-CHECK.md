# Registry check — 18 November 2026

Use the [project package guides](README.md#catalogue-des-sdk) for exact names and commands. This dated check makes the distinction between a prepared SDK and an installable registry version explicit.

Public version metadata was found for every package that the manifests mark as published in the checked registries. The 37 newly submitted Packagist packages also passed a clean real Composer installation, with the source commit, README, API guide, licence and native payload checked. No native SDK DLL was loaded and no driver operation was performed in those installation checks.

| Registry or listing | Entries checked | Expected metadata found |
| --- | ---: | ---: |
| NuGet | 76 | 76 |
| PyPI | 38 | 2 |
| npm | 38 | 10 |
| pub.dev | 38 | 24 |
| RubyGems | 38 | 38 |
| Packagist | 38 | 38 |
| crates.io | 38 | 38 |
| Maven Central | 38 | 38 |
| Go Modules proxy | 38 | 38 |
| pkg.go.dev | 38 | 38 |
| PowerShell Gallery | 38 | 38 |

NuGet includes 38 native source packages and 38 managed SDKs. The Go proxy and pkg.go.dev rows check the same 38 modules through different services; they are not additional products. A pkg.go.dev HTTP page is a listing check, not a compiled consumer test.

## Prepared packages still absent from the checked public registry

The following names returned HTTP404 at their recorded check time. Their guides retain the “after publication” wording. A source folder, downloadable archive or passing local fixture does not make a registry install command available.

### PyPI — 36 names

`autohotinterceptor`, `changed-block-tracking-sdk`, `cloud-files-sdk`, `connection-trace-studio`, `file-trace-studio`, `fyradevice`, `fyrainput`, `fyrainput-gamepad`, `fyrainput-keyboard-mouse`, `hid-access-control`, `interception-v2`, `interceptor-sdk`, `interceptor-gamepad-controller`, `interceptor-keyboard-mouse`, `network-emulation-sdk`, `node-interceptor`, `packet-capture-sdk`, `process-trace-studio`, `processguard-sdk`, `registryguard`, `remote-workstation-sdk`, `storage-io-flight-recorder`, `titan-audio-link`, `titan-file-filter`, `titan-network-filter`, `usb-over-ip-sdk`, `usb-trace-sdk`, `user-backed-virtual-disk-sdk`, `vdevice`, `virtual-camera-sdk`, `virtual-network-adapter-sdk`, `virtual-storage-fault-lab`, `windows-gamepad-filter`, `windows-input-filter`, `windows-keyboard-mouse-filter`, `windows-virtual-devices`.

### npm — 28 names

`changed-block-tracking-sdk`, `cloud-files-sdk`, `connection-trace-studio`, `file-trace-studio`, `fyradevice`, `hid-access-control`, `interception-v2`, `network-emulation-sdk`, `packet-capture-sdk`, `process-trace-studio`, `processguard-sdk`, `registryguard`, `remote-workstation-sdk`, `storage-io-flight-recorder`, `titan-audio-link`, `titan-file-filter`, `titan-network-filter`, `usb-over-ip-sdk`, `usb-trace-sdk`, `user-backed-virtual-disk-sdk`, `vdevice`, `virtual-camera-sdk`, `virtual-network-adapter-sdk`, `virtual-storage-fault-lab`, `windows-gamepad-filter`, `windows-input-filter`, `windows-keyboard-mouse-filter`, `windows-virtual-devices`.

### pub.dev — 14 names

`changed_block_tracking_sdk`, `cloud_files_sdk`, `hid_access_control`, `interception_v2`, `network_emulation_sdk`, `node_interceptor`, `packet_capture_sdk`, `remote_workstation_sdk`, `titan_audio_link`, `usb_over_ip_sdk`, `usb_trace_sdk`, `user_backed_virtual_disk_sdk`, `virtual_camera_sdk`, `virtual_network_adapter_sdk`.

## Scope and evidence

The [machine-readable check](reports/registry-metadata-2026-11-18.json) records each URL, version, result and check time. It contains 456 checks over 38 SDK projects; it does not mean 456 different products or 456 newly published packages. These are publisher-run checks, not reviews or independent certification.

vcpkg and Conan use the TitanCSA registries documented in each guide. The TitanCSA Conda channel is separate from official conda-forge acceptance. JitPack build status, those custom registries/channels and community-review acceptance were not newly live-qualified by this report. Existing publication receipts remain separate evidence.

A registry package installs a user-mode client, source or recipe. Matching application/driver installation, Windows x64 requirements, FFI permissions, edition compatibility, preview restrictions and licensing remain those stated by the project. Successful metadata retrieval or package extraction does not establish live driver behavior.
