vcpkg_download_distfile(ARCHIVE
    URLS "https://raw.githubusercontent.com/TitanCSA/sdk-packages/f6abf965e5a3e229c1b450322212aa07ec5a4cfc/release-assets/Interceptor-MultiLang/sdk-v1.0.0/interceptor-multilang-sdk-1.0.0.tar.gz"
    FILENAME "interceptor-multilang-sdk-1.0.0.tar.gz"
    SHA512 74876d401988b14b61490c72c70db69a2b37fc4c304f5f9efc0ce730aa89057522c9762939745eee114f4dff3622c1535b9c67f557131decf20d703b07a8ab8b
)
vcpkg_extract_source_archive(SOURCE_PATH ARCHIVE "${ARCHIVE}" NO_REMOVE_ONE_LEVEL)
vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME InterceptorMultiLangSDK CONFIG_PATH lib/cmake/InterceptorMultiLangSDK)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include" "${CURRENT_PACKAGES_DIR}/debug/share")
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
