# SDK publication status

Current source layout and registry coverage. A GitHub download is distinct from publication on a central registry.

| Target | Status | Package / documentation |
| --- | --- | --- |
| CPP/vcpkg | published_custom_registry | fyrainput-gamepad |
| CPP/conan | published_custom_source_index | [fyrainput-gamepad](https://github.com/TitanCSA/sdk-packages/tree/089bc73095aa8eda89bacbe10d0b9e58394b0207/conan-index) [Source](packaging/native/) |
| CPP/nuget | published_payload_verified | [FyraInput-Gamepad.Native](https://www.nuget.org/packages/FyraInput-Gamepad.Native/0.1.0) |
| NET/nuget | published_payload_verified | [FyraInputGamepad.SDK](https://www.nuget.org/packages/FyraInputGamepad.SDK/1.0.0) |
| Python/pypi | built_fixture_and_production_verified | fyrainput-gamepad [Source](FyraInput-Gamepad-Python-pypi/) |
| Python/conda-forge | custom_channel_install_verified_requires_conda_forge_review | [fyrainput-gamepad](https://raw.githubusercontent.com/TitanCSA/sdk-packages/089bc73095aa8eda89bacbe10d0b9e58394b0207/conda-channel) [Source](packaging/conda/) |
| Rust/cargo | published_install_verified | [fyrainput-gamepad](https://crates.io/crates/fyrainput-gamepad) [Source](FyraInput-Gamepad-Rust-cargo/) |
| JavaScript/npm | published_install_verified | [fyrainput-gamepad](https://www.npmjs.com/package/fyrainput-gamepad) [Source](FyraInput-Gamepad-JavaScript-npm/) |
| Java/maven | published_install_verified | [fyrainput-gamepad](https://central.sonatype.com/artifact/io.github.titancsa/fyrainput-gamepad/1.0.0) [Source](FyraInput-Gamepad-Java-maven/) |
| Java/jitpack | published_install_verified | [fyrainput-gamepad](https://jitpack.io/#TitanCSA/FyraInput-Gamepad-SDK/java-sdk-v1.0.0) [Source](FyraInput-Gamepad-Java-maven/) |
| Go/modules | published_install_verified | [fyrainput-gamepad](https://pkg.go.dev/github.com/TitanCSA/FyraInput-Gamepad-SDK/FyraInput-Gamepad-Go-modules@v1.0.0) [Source](FyraInput-Gamepad-Go-modules/) |
| Go/pkg-go-dev | indexed_verified | [Documentation](https://pkg.go.dev/github.com/TitanCSA/FyraInput-Gamepad-SDK/FyraInput-Gamepad-Go-modules@v1.0.0) |
| PowerShell/gallery | published_install_verified | [FyraInputGamepad](https://www.powershellgallery.com/packages/FyraInputGamepad/1.0.0) [Source](FyraInput-Gamepad-PowerShell-gallery/) |
| Dart/pub-dev | published_install_verified | [fyrainput_gamepad](https://pub.dev/packages/fyrainput_gamepad) [Source](FyraInput-Gamepad-Dart-pub-dev/) |
| Ruby/rubygems | published_install_verified | [fyrainput-gamepad](https://rubygems.org/gems/fyrainput-gamepad) [Source](FyraInput-Gamepad-Ruby-rubygems/) |
| PHP/packagist | composer_source_and_tag_published | [titancsa/fyrainput-gamepad](https://packagist.org/packages/titancsa/fyrainput-gamepad) |

Native SDK source and CMake/Conan/vcpkg/NuGet recipes share [`packaging/native`](packaging/native/LAYOUT.md). Previous release archives and tags remain unchanged.

Application and driver prerequisites and original licensing remain those documented by each SDK. No driver installer is bundled with these source packages.
