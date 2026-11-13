vcpkg_download_distfile(ARCHIVE
    URLS "https://raw.githubusercontent.com/TitanCSA/sdk-packages/f6abf965e5a3e229c1b450322212aa07ec5a4cfc/release-assets/FyraInput-Keyboard-Mouse/sdk-v0.1.0/fyrainput-keyboard-mouse-sdk-0.1.0.tar.gz"
    FILENAME "fyrainput-keyboard-mouse-sdk-0.1.0.tar.gz"
    SHA512 fb8ca8089e08750a2527aaefb68e2d35b6cb14c34073fd3256318590fbea5bb7ecab864de1c5cc59dcdc2f4cbab49a78f8fb0dea635e991a569e5b579d111c91
)
vcpkg_extract_source_archive(SOURCE_PATH ARCHIVE "${ARCHIVE}" NO_REMOVE_ONE_LEVEL)
vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME FyraInputKeyboardMouseSDK CONFIG_PATH lib/cmake/FyraInputKeyboardMouseSDK)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include" "${CURRENT_PACKAGES_DIR}/debug/share")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
