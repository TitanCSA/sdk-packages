using TitanCSA.Interceptor;

Console.WriteLine("Invert mouse Y. Close the program to restore normal input.");
using var io = new Interceptor();
while (true) {
    var inputs = io.Receive();
    foreach (var input in inputs)
        if (input.Kind == "mouse" && (input.State["flags"] & 1) == 0) input.State["y"] = -input.State["y"];
    io.Send(inputs);
}
