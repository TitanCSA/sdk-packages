# Validation

Windows validation performed on 30 October 2026. These are measured checks, not a claim that every device, AutoHotkey script or Windows configuration is bug-free.

- Native x64/x86 synthetic tests exercise routing, signed field conversion, source identity, filters, cancellation, capture conflicts, invalid handles, repeated startup/shutdown, 20,000 receive/forward iterations and overflow release (over 180,000 checks per run).
- The native suite is also run with MSVC AddressSanitizer for memory-error detection.
- Managed tests exercise mixed keyboards/mouse input, subscription changes, context callbacks, callback failures and extended scan-code round trips.
- Stock AutoHotkey v1.1.37.02 and v2.0.31 smoke tests pass on both Unicode x86 and x64 runtimes.
- A disposable Windows 11 VM runs the unchanged Microsoft-signed Keyboard & Mouse driver together with owned Vdevice HID fixtures. Two independent keyboards and two mice generate reports. AutoHotkey remaps A to B on one keyboard and inverts one mouse's Y axis; an independent Windows Raw Input observer confirms the transformed reports, while the unselected devices pass through.

Physical USB, PS/2, Bluetooth, vendor software and other Windows versions have not all been exercised by these tests. Real supported sources depend on this driver's Windows stack support. Existing runtime and driver limitations are in the main README. Monitoring display rows are bounded and are not a complete capture/performance trace.
