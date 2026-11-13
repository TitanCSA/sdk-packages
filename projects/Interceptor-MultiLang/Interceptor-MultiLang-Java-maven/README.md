# Interceptor Java and Kotlin SDK

Windows x64 Java 22+ FFM bindings for the combined Interceptor 1.4.22.0 driver.
Kotlin projects can consume the same Java API. Driver installation is separate.

Maven coordinates: `io.github.titancsa:interceptor:1.0.1`

```xml
<dependency>
  <groupId>io.github.titancsa</groupId>
  <artifactId>interceptor</artifactId>
  <version>1.0.1</version>
</dependency>
```

These coordinates become installable after successful Maven Central publication.
The JAR contains the native bridge DLL, extracted automatically on first use.
Launch with `--enable-native-access=ALL-UNNAMED` on Windows x64.

```java
import titancsa.interceptor.Interceptor;
try (var io = new Interceptor()) {
    System.out.println("SDK loaded; capture has not been started");
}
```

Capture requires the separately installed matching driver and administrator rights.
Forward every received input batch or explicitly mark blocked events. Driver demo
restrictions and licensing remain separate from the MIT wrapper.

Documentation is bundled in META-INF/docs/API.md, with a usage example and LICENSE.
A separate sources JAR and generated Javadoc JAR are included in the distribution.
Source: https://github.com/TitanCSA/Interceptor-MultiLang
