cmake_minimum_required(VERSION 3.24)
project(FyraInputGamepadSDK VERSION 0.1.0.0 LANGUAGES C CXX)
if(NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR "This SDK requires Windows x64. No driver is installed by this build.")
endif()
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
add_library(TitanFyraInputGamepad STATIC
  "sdk/FyraInput/FyraInput.cpp"
)
add_library(FyraInputGamepadSDK::sdk ALIAS TitanFyraInputGamepad)
set_target_properties(TitanFyraInputGamepad PROPERTIES EXPORT_NAME sdk)
target_compile_features(TitanFyraInputGamepad PUBLIC cxx_std_17)
target_compile_definitions(TitanFyraInputGamepad PUBLIC )
target_compile_definitions(TitanFyraInputGamepad PRIVATE UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0A00)
if(MSVC)
  target_compile_options(TitanFyraInputGamepad PRIVATE /utf-8 /EHsc /W4)
endif()
target_include_directories(TitanFyraInputGamepad PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdk>"
  "$<INSTALL_INTERFACE:include/>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdk/FyraInput>"
  "$<INSTALL_INTERFACE:include/FyraInput>"
)
target_link_libraries(TitanFyraInputGamepad PUBLIC advapi32 cfgmgr32 comctl32 hid newdev ole32 setupapi user32)
install(TARGETS TitanFyraInputGamepad EXPORT FyraInputGamepadSDKTargets ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY sdk/ DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl")
install(FILES LICENSE DESTINATION share/fyrainput-gamepad)
install(EXPORT FyraInputGamepadSDKTargets NAMESPACE FyraInputGamepadSDK:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FyraInputGamepadSDK)
configure_file("FyraInputGamepadSDKConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/FyraInputGamepadSDKConfig.cmake" @ONLY)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/FyraInputGamepadSDKConfigVersion.cmake" VERSION 0.1.0.0 COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/FyraInputGamepadSDKConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/FyraInputGamepadSDKConfigVersion.cmake" DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FyraInputGamepadSDK)
