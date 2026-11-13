from interceptor import Interceptor

print("Send 200 relative mouse units through Interceptor.")
with Interceptor() as io:
    io.inject("mouse", x=200)
