# TitanCSA Conan source index

Conan2 local-recipes-index, published as Git source. This is separate from ConanCenter and from an HTTP binary package server. Recipes download pinned SHA256 source archives and build the native SDK. No driver is installed. SDK licenses and prerequisites remain unchanged.

```powershell
git clone https://github.com/TitanCSA/sdk-packages.git
conan remote add titancsa ./sdk-packages/conan-index -t local-recipes-index
conan install --requires=interceptor/1.4.22 -r titancsa --build=missing -s compiler.cppstd=17
```

Use a version listed in `packages.json`; package names match the project names in lowercase. Original `*-sdk` names remain available. Build source and CMake dependency metadata are the same as the native SDK recipes. Windows x64 MSVC is required. Consumers should pin the Git commit of this index. Source archives, prior versions and licenses are preserved.

Mechanism: https://docs.conan.io/2/devops/devops_local_recipes_index.html

Virtual-Camera-SDK requires `-s compiler.cppstd=20`; other SDKs require C++17.
