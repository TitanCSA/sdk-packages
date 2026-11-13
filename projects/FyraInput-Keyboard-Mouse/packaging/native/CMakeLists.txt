cmake_minimum_required(VERSION 3.24)
project(FyraInputKeyboardMouseSDK VERSION 0.1.0.0 LANGUAGES C CXX)
if(NOT WIN32 OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR "This SDK requires Windows x64. No driver is installed by this build.")
endif()
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
add_library(TitanFyraInputKeyboardMouse STATIC
  "sdk/FyraInput/FyraInput.cpp"
)
add_library(FyraInputKeyboardMouseSDK::sdk ALIAS TitanFyraInputKeyboardMouse)
set_target_properties(TitanFyraInputKeyboardMouse PROPERTIES EXPORT_NAME sdk)
target_compile_features(TitanFyraInputKeyboardMouse PUBLIC cxx_std_17)
target_compile_definitions(TitanFyraInputKeyboardMouse PUBLIC )
target_compile_definitions(TitanFyraInputKeyboardMouse PRIVATE UNICODE _UNICODE NOMINMAX WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0A00)
if(MSVC)
  target_compile_options(TitanFyraInputKeyboardMouse PRIVATE /utf-8 /EHsc /W4)
endif()
target_include_directories(TitanFyraInputKeyboardMouse PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdk>"
  "$<INSTALL_INTERFACE:include/>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/sdk/FyraInput>"
  "$<INSTALL_INTERFACE:include/FyraInput>"
)
target_link_libraries(TitanFyraInputKeyboardMouse PUBLIC advapi32 cfgmgr32 comctl32 hid newdev ole32 setupapi user32)
install(TARGETS TitanFyraInputKeyboardMouse EXPORT FyraInputKeyboardMouseSDKTargets ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY sdk/ DESTINATION include FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.inl")
install(FILES LICENSE DESTINATION share/fyrainput-keyboard-mouse)
install(EXPORT FyraInputKeyboardMouseSDKTargets NAMESPACE FyraInputKeyboardMouseSDK:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FyraInputKeyboardMouseSDK)
configure_file("FyraInputKeyboardMouseSDKConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/FyraInputKeyboardMouseSDKConfig.cmake" @ONLY)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/FyraInputKeyboardMouseSDKConfigVersion.cmake" VERSION 0.1.0.0 COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/FyraInputKeyboardMouseSDKConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/FyraInputKeyboardMouseSDKConfigVersion.cmake" DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FyraInputKeyboardMouseSDK)
