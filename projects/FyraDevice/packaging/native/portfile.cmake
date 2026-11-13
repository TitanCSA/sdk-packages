vcpkg_download_distfile(ARCHIVE
    URLS "https://github.com/TitanCSA/FyraDevice-SDK/releases/download/sdk-v0.1.0/fyradevice-sdk-0.1.0.tar.gz"
    FILENAME "fyradevice-sdk-0.1.0.tar.gz"
    SHA512 fd32e87c88ea6959c2fc113db4c0732b6eceaa924c947370dfcaedfcbc1349f61a684189e4d7f5522d9a8b2ddfaceaf4b5ae5802e28f468529a258b00461f60c
)
vcpkg_extract_source_archive(SOURCE_PATH ARCHIVE "${ARCHIVE}" NO_REMOVE_ONE_LEVEL)
vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME FyraDeviceSDK CONFIG_PATH lib/cmake/FyraDeviceSDK)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include" "${CURRENT_PACKAGES_DIR}/debug/share")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
