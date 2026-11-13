vcpkg_download_distfile(ARCHIVE
    URLS "https://raw.githubusercontent.com/TitanCSA/sdk-packages/f6abf965e5a3e229c1b450322212aa07ec5a4cfc/release-assets/HID-Access-Control/sdk-v0.1.0/hid-access-control-sdk-0.1.0.tar.gz"
    FILENAME "hid-access-control-sdk-0.1.0.tar.gz"
    SHA512 8f0ae9e0c0484600d23563b9f7e47d18e400492ed48b1e38405615408a07fb1a51ab5457de446fc740693fa77891e14954f0ac0d94d45989a356eb2b54641d17
)
vcpkg_extract_source_archive(SOURCE_PATH ARCHIVE "${ARCHIVE}" NO_REMOVE_ONE_LEVEL)
vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME HIDAccessControlSDK CONFIG_PATH lib/cmake/HIDAccessControlSDK)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include" "${CURRENT_PACKAGES_DIR}/debug/share")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
