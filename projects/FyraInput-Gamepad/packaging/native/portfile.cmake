vcpkg_download_distfile(ARCHIVE
    URLS "https://github.com/TitanCSA/FyraInput-Gamepad-SDK/releases/download/sdk-v0.1.0/fyrainput-gamepad-sdk-0.1.0.tar.gz"
    FILENAME "fyrainput-gamepad-sdk-0.1.0.tar.gz"
    SHA512 404190f163dcd8b05b7c50011598b1bde44f75b21c30fe8562195408454d220e8961d7a846ee4ce13882d0cccc41f5ac59139a99b92e2470511f5d340be7b17c
)
vcpkg_extract_source_archive(SOURCE_PATH ARCHIVE "${ARCHIVE}" NO_REMOVE_ONE_LEVEL)
vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME FyraInputGamepadSDK CONFIG_PATH lib/cmake/FyraInputGamepadSDK)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include" "${CURRENT_PACKAGES_DIR}/debug/share")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
