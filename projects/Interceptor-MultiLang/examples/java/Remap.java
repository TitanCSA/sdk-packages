import titancsa.interceptor.Interceptor;
class Remap {
    public static void main(String[] args) {
        System.out.println("Invert mouse Y. Close the program to restore normal input.");
        try (var io = new Interceptor()) {
            while (true) {
                var inputs = io.receive();
                for (var input : inputs)
                    if (input.kind.equals("mouse") && (input.state.scalars.get("flags") & 1) == 0)
                        input.state.scalars.put("y", -input.state.scalars.get("y"));
                io.send(inputs);
            }
        }
    }
}
