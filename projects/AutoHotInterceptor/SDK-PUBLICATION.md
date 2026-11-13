# SDK publication status

Current source layout and registry coverage. A GitHub download is distinct from publication on a central registry.

| Target | Status | Package / documentation |
| --- | --- | --- |
| CPP/vcpkg | published_custom_registry | autohotinterceptor |
| CPP/conan | published_custom_source_index | [autohotinterceptor](https://github.com/TitanCSA/sdk-packages/tree/089bc73095aa8eda89bacbe10d0b9e58394b0207/conan-index) [Source](packaging/native/) |
| CPP/nuget | published_payload_verified | [AutoHotInterceptor.Native](https://www.nuget.org/packages/AutoHotInterceptor.Native/1.0.0) |
| NET/nuget | published_payload_verified | [AutoHotInterceptor](https://www.nuget.org/packages/AutoHotInterceptor/1.0.1) |
| Python/pypi | built_fixture_and_production_verified | autohotinterceptor [Source](AutoHotInterceptor-Python-pypi/) |
| Python/conda-forge | custom_channel_install_verified_requires_conda_forge_review | [autohotinterceptor](https://raw.githubusercontent.com/TitanCSA/sdk-packages/089bc73095aa8eda89bacbe10d0b9e58394b0207/conda-channel) [Source](packaging/conda/) |
| Rust/cargo | published_install_verified | [autohotinterceptor](https://crates.io/crates/autohotinterceptor) [Source](AutoHotInterceptor-Rust-cargo/) |
| JavaScript/npm | published_install_verified | [autohotinterceptor](https://www.npmjs.com/package/autohotinterceptor) [Source](AutoHotInterceptor-JavaScript-npm/) |
| Java/maven | published_install_verified | [autohotinterceptor](https://central.sonatype.com/artifact/io.github.titancsa/autohotinterceptor/1.0.0) [Source](AutoHotInterceptor-Java-maven/) |
| Java/jitpack | published_install_verified | [autohotinterceptor](https://jitpack.io/#TitanCSA/AutoHotInterceptor/java-sdk-v1.0.0) [Source](AutoHotInterceptor-Java-maven/) |
| Go/modules | published_install_verified | [autohotinterceptor](https://pkg.go.dev/github.com/TitanCSA/AutoHotInterceptor/AutoHotInterceptor-Go-modules@v1.0.0) [Source](AutoHotInterceptor-Go-modules/) |
| Go/pkg-go-dev | indexed_verified | [Documentation](https://pkg.go.dev/github.com/TitanCSA/AutoHotInterceptor/AutoHotInterceptor-Go-modules@v1.0.0) |
| PowerShell/gallery | published_install_verified | [AutoHotInterceptor](https://www.powershellgallery.com/packages/AutoHotInterceptor/1.0.0) [Source](AutoHotInterceptor-PowerShell-gallery/) |
| Dart/pub-dev | published_install_verified | [autohotinterceptor](https://pub.dev/packages/autohotinterceptor) [Source](AutoHotInterceptor-Dart-pub-dev/) |
| Ruby/rubygems | published_install_verified | [autohotinterceptor](https://rubygems.org/gems/autohotinterceptor) [Source](AutoHotInterceptor-Ruby-rubygems/) |
| PHP/packagist | composer_source_and_tag_published | [titancsa/autohotinterceptor](https://packagist.org/packages/titancsa/autohotinterceptor) |

Native SDK source and CMake/Conan/vcpkg/NuGet recipes share [`packaging/native`](packaging/native/LAYOUT.md). Previous release archives and tags remain unchanged.

Application and driver prerequisites and original licensing remain those documented by each SDK. No driver installer is bundled with these source packages.
