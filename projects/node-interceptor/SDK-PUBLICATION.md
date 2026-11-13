# SDK publication status

Current source layout and registry coverage. A GitHub download is distinct from publication on a central registry.

| Target | Status | Package / documentation |
| --- | --- | --- |
| CPP/vcpkg | published_custom_registry | node-interceptor |
| CPP/conan | published_custom_source_index | [node-interceptor](https://github.com/TitanCSA/sdk-packages/tree/089bc73095aa8eda89bacbe10d0b9e58394b0207/conan-index) |
| CPP/nuget | published_payload_verified | [NodeInterceptor.Native](https://www.nuget.org/packages/NodeInterceptor.Native/1.0.0) |
| NET/nuget | published_payload_verified | [NodeInterceptor.SDK](https://www.nuget.org/packages/NodeInterceptor.SDK/1.0.0) |
| Python/pypi | built_fixture_and_production_verified | node-interceptor [Source](node-interceptor-Python-pypi/) |
| Python/conda-forge | custom_channel_install_verified_requires_conda_forge_review | [node-interceptor](https://raw.githubusercontent.com/TitanCSA/sdk-packages/089bc73095aa8eda89bacbe10d0b9e58394b0207/conda-channel) [Source](packaging/conda/) |
| Rust/cargo | published_install_verified | [node-interceptor](https://crates.io/crates/node-interceptor) [Source](node-interceptor-Rust-cargo/) |
| JavaScript/npm | published_install_verified | [interceptor-keyboard-mouse](https://www.npmjs.com/package/interceptor-keyboard-mouse) [Source](node-interceptor-JavaScript-npm/) |
| Java/maven | published_install_verified | [node-interceptor](https://central.sonatype.com/artifact/io.github.titancsa/node-interceptor/1.0.0) [Source](node-interceptor-Java-maven/) |
| Java/jitpack | published_install_verified | [node-interceptor](https://jitpack.io/#TitanCSA/node-interceptor/java-sdk-v1.0.0) [Source](node-interceptor-Java-maven/) |
| Go/modules | published_install_verified | [node-interceptor](https://pkg.go.dev/github.com/TitanCSA/node-interceptor/node-interceptor-Go-modules@v1.0.0) [Source](node-interceptor-Go-modules/) |
| Go/pkg-go-dev | indexed_verified | [Documentation](https://pkg.go.dev/github.com/TitanCSA/node-interceptor/node-interceptor-Go-modules@v1.0.0) |
| PowerShell/gallery | published_install_verified | [NodeInterceptor](https://www.powershellgallery.com/packages/NodeInterceptor/1.0.0) [Source](node-interceptor-PowerShell-gallery/) |
| Dart/pub-dev | built_fixture_and_production_verified | node_interceptor [Source](node-interceptor-Dart-pub-dev/) |
| Ruby/rubygems | published_install_verified | [node-interceptor](https://rubygems.org/gems/node-interceptor) [Source](node-interceptor-Ruby-rubygems/) |
| PHP/packagist | composer_source_and_tag_published | [titancsa/node-interceptor](https://packagist.org/packages/titancsa/node-interceptor) |

Native SDK source and CMake/Conan/vcpkg/NuGet recipes share [`packaging/native`](packaging/native/LAYOUT.md). Previous release archives and tags remain unchanged.

Application and driver prerequisites and original licensing remain those documented by each SDK. No driver installer is bundled with these source packages.
