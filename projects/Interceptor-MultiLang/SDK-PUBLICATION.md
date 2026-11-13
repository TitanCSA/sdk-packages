# SDK publication status

Current source layout and registry coverage. A GitHub download is distinct from publication on a central registry.

| Target | Status | Package / documentation |
| --- | --- | --- |
| CPP/vcpkg | published_custom_registry | interceptor-multilang |
| CPP/conan | published_custom_source_index | [interceptor-multilang](https://github.com/TitanCSA/sdk-packages/tree/089bc73095aa8eda89bacbe10d0b9e58394b0207/conan-index) [Source](packaging/native/) |
| CPP/nuget | published_payload_verified | [Interceptor-MultiLang.Native](https://www.nuget.org/packages/Interceptor-MultiLang.Native/1.0.0) |
| NET/nuget | published_payload_verified | [Interceptor.MultiLang](https://www.nuget.org/packages/Interceptor.MultiLang/1.0.1) |
| Python/pypi | published_install_verified | [interceptor-multilang](https://pypi.org/project/interceptor-multilang/1.0.1/) |
| Python/conda-forge | submitted_pending_maintainer_review | interceptor-multilang [Source](Interceptor-MultiLang-Python-conda-forge/) |
| Rust/cargo | published_install_verified | [interceptor-multilang](https://crates.io/crates/interceptor-multilang) |
| JavaScript/npm | published_install_verified | [interceptor-multilang](https://www.npmjs.com/package/interceptor-multilang) [Source](Interceptor-MultiLang-JavaScript-npm/) |
| Java/maven | published_install_verified | [io.github.titancsa:interceptor:1.0.1](https://central.sonatype.com/artifact/io.github.titancsa/interceptor/1.0.1) |
| Java/jitpack | published_verified | com.github.TitanCSA:Interceptor-MultiLang:java-sdk-v1.0.1 |
| Go/modules | published_verified | github.com/TitanCSA/Interceptor-MultiLang/Interceptor-MultiLang-Go-modules |
| Go/pkg-go-dev | indexed_verified | [Documentation](https://pkg.go.dev/github.com/TitanCSA/Interceptor-MultiLang/Interceptor-MultiLang-Go-modules@v1.0.0) |
| PowerShell/gallery | published_install_verified | [InterceptorMultiLang](https://www.powershellgallery.com/packages/InterceptorMultiLang/1.0.0) [Source](Interceptor-MultiLang-PowerShell-gallery/) |
| Dart/pub-dev | published_install_verified | [interceptor_multilang](https://pub.dev/packages/interceptor_multilang) [Source](Interceptor-MultiLang-Dart-pub-dev/) |
| Ruby/rubygems | published_install_verified | [interceptor-multilang](https://rubygems.org/gems/interceptor-multilang) [Source](Interceptor-MultiLang-Ruby-rubygems/) |
| PHP/packagist | published_install_verified | [titancsa/interceptor-multilang](https://packagist.org/packages/titancsa/interceptor-multilang) [Source](Interceptor-MultiLang-PHP-packagist/) |

Native SDK source and CMake/Conan/vcpkg/NuGet recipes share [`packaging/native`](packaging/native/LAYOUT.md). Previous release archives and tags remain unchanged.

Application and driver prerequisites and original licensing remain those documented by each SDK. No driver installer is bundled with these source packages.
