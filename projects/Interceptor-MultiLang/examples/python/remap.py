from interceptor import Interceptor

print("Invert mouse Y. Press Ctrl+C to restore normal input.")
with Interceptor() as io:
    while True:
        inputs = io.receive()
        for event in inputs:
            if event.kind == "mouse" and not event.state.flags & 1:
                event.state.y = -event.state.y
        io.send(inputs)  # Forward every family, including unchanged inputs.
