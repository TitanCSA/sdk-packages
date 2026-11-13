from interceptor import Interceptor

print("Invert vertical gamepad axes. Press Ctrl+C to restore normal input.")
with Interceptor() as io:
    while True:
        inputs = io.receive()
        for event in inputs:
            if event.kind in ("hid", "xinputhid"):
                for axis in event.state.get("axes", []):
                    if axis.usage_page == 1 and axis.usage in (0x31, 0x34) and axis.normalized_valid:
                        axis.normalized = -axis.normalized
            elif event.kind in ("xinput", "gip"):
                event.state.left_y = min(32767, -event.state.left_y)
                event.state.right_y = min(32767, -event.state.right_y)
        io.send(inputs)
