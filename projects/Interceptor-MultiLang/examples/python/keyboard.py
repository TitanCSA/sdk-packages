from interceptor import Interceptor

print("Remap Caps Lock to Escape. Press Ctrl+C to restore normal input.")
with Interceptor() as io:
    while True:
        inputs = io.receive()
        for event in inputs:
            if event.kind == "keyboard" and event.state.scan_code == 0x3A:
                event.state.scan_code = 0x01
        io.send(inputs)
