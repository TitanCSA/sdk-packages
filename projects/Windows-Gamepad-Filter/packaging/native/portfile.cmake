vcpkg_download_distfile(ARCHIVE
    URLS "https://raw.githubusercontent.com/TitanCSA/sdk-packages/f6abf965e5a3e229c1b450322212aa07ec5a4cfc/release-assets/Windows-Gamepad-Filter/sdk-v0.1.0/windows-gamepad-filter-sdk-0.1.0.tar.gz"
    FILENAME "windows-gamepad-filter-sdk-0.1.0.tar.gz"
    SHA512 b6f1b4103f956ff00a8f8563ebf397a3eb241e66257ac9c402a44ad8513ddc6b907fdc199f03ff4e85333397f7c5dc3fe43240ce9c578169aba283e810a781e5
)
vcpkg_extract_source_archive(SOURCE_PATH ARCHIVE "${ARCHIVE}" NO_REMOVE_ONE_LEVEL)
vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME WindowsGamepadFilterSDK CONFIG_PATH lib/cmake/WindowsGamepadFilterSDK)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include" "${CURRENT_PACKAGES_DIR}/debug/share")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
