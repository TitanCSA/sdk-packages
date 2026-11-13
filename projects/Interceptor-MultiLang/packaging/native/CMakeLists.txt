cmake_minimum_required(VERSION 3.24)
project(InterceptorMultiLangSDK VERSION 1.0.0 LANGUAGES C CXX)
if(NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR "This SDK requires Windows x64. No driver is installed by this build.")
endif()
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
add_library(TitanInterceptorMultiLang STATIC
  "sdk/native/Bridge.cpp"
  "sdk/sdk/Interceptor.cpp"
)
add_library(InterceptorMultiLangSDK::sdk ALIAS TitanInterceptorMultiLang)
set_target_properties(TitanInterceptorMultiLang PROPERTIES EXPORT_NAME sdk)
target_compile_features(TitanInterceptorMultiLang PUBLIC cxx_std_17)
target_compile_definitions(TitanInterceptorMultiLang PUBLIC IC_BRIDGE_STATIC)
target_compile_definitions(TitanInterceptorMultiLang PRIVATE UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0A00)
if(MSVC)
  target_compile_options(TitanInterceptorMultiLang PRIVATE /utf-8 /EHsc /W4)
endif()
target_include_directories(TitanInterceptorMultiLang PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdk>"
  "$<INSTALL_INTERFACE:include/>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdk/native>"
  "$<INSTALL_INTERFACE:include/native>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdk/sdk>"
  "$<INSTALL_INTERFACE:include/sdk>"
)
target_link_libraries(TitanInterceptorMultiLang PUBLIC advapi32 bcrypt cfgmgr32 comctl32 hid newdev ole32 setupapi user32)
install(TARGETS TitanInterceptorMultiLang EXPORT InterceptorMultiLangSDKTargets ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY sdk/ DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl")
install(FILES LICENSE DESTINATION share/interceptor-multilang)
install(EXPORT InterceptorMultiLangSDKTargets NAMESPACE InterceptorMultiLangSDK:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/InterceptorMultiLangSDK)
configure_file("InterceptorMultiLangSDKConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/InterceptorMultiLangSDKConfig.cmake" @ONLY)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/InterceptorMultiLangSDKConfigVersion.cmake" VERSION 1.0.0 COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/InterceptorMultiLangSDKConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/InterceptorMultiLangSDKConfigVersion.cmake" DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/InterceptorMultiLangSDK)
