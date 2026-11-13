vcpkg_download_distfile(ARCHIVE
    URLS "https://raw.githubusercontent.com/TitanCSA/sdk-packages/f6abf965e5a3e229c1b450322212aa07ec5a4cfc/release-assets/AutoHotInterceptor/sdk-v1.0.0/autohotinterceptor-sdk-1.0.0.tar.gz"
    FILENAME "autohotinterceptor-sdk-1.0.0.tar.gz"
    SHA512 1a2943a6e39692714e7ec9bc56233a6c88fa1d4664a72d4dce007929cc44369a789e64c18d27d88bc79720c5bcb3f1eb426a99d9c4f956561628c1e4872315ee
)
vcpkg_extract_source_archive(SOURCE_PATH ARCHIVE "${ARCHIVE}" NO_REMOVE_ONE_LEVEL)
vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME AutoHotInterceptorSDK CONFIG_PATH lib/cmake/AutoHotInterceptorSDK)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include" "${CURRENT_PACKAGES_DIR}/debug/share")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
