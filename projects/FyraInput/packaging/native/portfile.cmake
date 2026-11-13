vcpkg_download_distfile(ARCHIVE
    URLS "https://github.com/TitanCSA/FyraInput-SDK/releases/download/sdk-v0.1.0/fyrainput-sdk-0.1.0.tar.gz"
    FILENAME "fyrainput-sdk-0.1.0.tar.gz"
    SHA512 33a3bbd4179a5793830f5a8195625c979f225471c466403d05fe09c2e306275488bb05eb0a74aa4f14b40b40a7a4ac3ef8ec9ec012dda35b4bd2ac6e8533e4a9
)
vcpkg_extract_source_archive(SOURCE_PATH ARCHIVE "${ARCHIVE}" NO_REMOVE_ONE_LEVEL)
vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME FyraInputSDK CONFIG_PATH lib/cmake/FyraInputSDK)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include" "${CURRENT_PACKAGES_DIR}/debug/share")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
