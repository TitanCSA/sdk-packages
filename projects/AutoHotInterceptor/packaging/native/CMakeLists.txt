cmake_minimum_required(VERSION 3.24)
project(AutoHotInterceptorSDK VERSION 1.0.0 LANGUAGES C CXX)
if(NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR "This SDK requires Windows x64. No driver is installed by this build.")
endif()
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
add_library(TitanAutoHotInterceptor STATIC
  "sdk/Native/Bridge.cpp"
  "sdk/Native/SDK/Interceptor.cpp"
)
add_library(AutoHotInterceptorSDK::sdk ALIAS TitanAutoHotInterceptor)
set_target_properties(TitanAutoHotInterceptor PROPERTIES EXPORT_NAME sdk)
target_compile_features(TitanAutoHotInterceptor PUBLIC cxx_std_17)
target_compile_definitions(TitanAutoHotInterceptor PUBLIC INTERCEPTION_STATIC)
target_compile_definitions(TitanAutoHotInterceptor PRIVATE UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0A00)
if(MSVC)
  target_compile_options(TitanAutoHotInterceptor PRIVATE /utf-8 /EHsc /W4)
endif()
target_include_directories(TitanAutoHotInterceptor PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdk>"
  "$<INSTALL_INTERFACE:include/>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdk/Native>"
  "$<INSTALL_INTERFACE:include/Native>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdk/Native/SDK>"
  "$<INSTALL_INTERFACE:include/Native/SDK>"
)
target_link_libraries(TitanAutoHotInterceptor PUBLIC advapi32 bcrypt cfgmgr32 comctl32 hid newdev ole32 setupapi user32)
install(TARGETS TitanAutoHotInterceptor EXPORT AutoHotInterceptorSDKTargets ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY sdk/ DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl")
install(FILES LICENSE DESTINATION share/autohotinterceptor)
install(EXPORT AutoHotInterceptorSDKTargets NAMESPACE AutoHotInterceptorSDK:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AutoHotInterceptorSDK)
configure_file("AutoHotInterceptorSDKConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/AutoHotInterceptorSDKConfig.cmake" @ONLY)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/AutoHotInterceptorSDKConfigVersion.cmake" VERSION 1.0.0 COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/AutoHotInterceptorSDKConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/AutoHotInterceptorSDKConfigVersion.cmake" DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/AutoHotInterceptorSDK)
