import titancsa.interceptor.Interceptor;
public class Smoke { public static void main(String[] args) { try (var io = new Interceptor()) { System.out.println("Packaged SDK lazy open/close passed"); } } }
