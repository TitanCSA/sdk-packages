vcpkg_download_distfile(ARCHIVE
    URLS "https://github.com/TitanCSA/node-interceptor/releases/download/sdk-native-v1.0.0/node-interceptor-sdk-1.0.0.tar.gz"
    FILENAME "node-interceptor-sdk-1.0.0.tar.gz"
    SHA512 55c7809b4678af022010d8ef807ab8fc5f3768faa07930feef466c65359ab9bbaaea19b80bcc97263a44fbc39b70b5691dd27c65e34c103075c6b97bfba5601e
)
vcpkg_extract_source_archive(SOURCE_PATH ARCHIVE "${ARCHIVE}" NO_REMOVE_ONE_LEVEL)
vcpkg_cmake_configure(SOURCE_PATH "${SOURCE_PATH}")
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME NodeInterceptorSDK CONFIG_PATH lib/cmake/NodeInterceptorSDK)
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include" "${CURRENT_PACKAGES_DIR}/debug/share")
file(GLOB_RECURSE EXTRA_COPYRIGHT "${SOURCE_PATH}/licenses/*.txt")
foreach(NOTICE_FILE IN ITEMS NOTICE NOTICE.txt)
    if(EXISTS "${SOURCE_PATH}/${NOTICE_FILE}")
        list(APPEND EXTRA_COPYRIGHT "${SOURCE_PATH}/${NOTICE_FILE}")
    endif()
endforeach()
vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE" ${EXTRA_COPYRIGHT})
