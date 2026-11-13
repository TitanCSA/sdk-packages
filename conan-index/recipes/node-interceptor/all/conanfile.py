from conan.tools.files import get
from conan.tools.files import copy
from os.path import join
from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, cmake_layout
class NativeSDK(ConanFile):
    name = 'node-interceptor'
    version = "1.0.0"
    license = "LGPL-3.0-or-later"
    description = "Public Windows x64 user-mode SDK for node-interceptor; driver not included."
    settings = "os", "arch", "compiler", "build_type"
    generators = "CMakeToolchain", "CMakeDeps"
    def validate(self):
        if str(self.settings.os) != "Windows" or str(self.settings.arch) != "x86_64":
            raise ConanInvalidConfiguration("Windows x64 is required")
    def layout(self):
        cmake_layout(self)
    def source(self):
        get(self, **self.conan_data["sources"][self.version], strip_root=False)
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
    def package(self):
        CMake(self).install()
        copy(self, "LICENSE", self.source_folder, join(self.package_folder, "licenses"))
        copy(self, "NOTICE*", self.source_folder, join(self.package_folder, "licenses"))
        copy(self, "*", join(self.source_folder, "licenses"), join(self.package_folder, "licenses", "upstream"))
    def package_info(self):
        self.cpp_info.defines = ["INTERCEPTION_STATIC"]
        self.cpp_info.libs = ["TitanNodeInterceptor"]
        self.cpp_info.includedirs = ['include']
        self.cpp_info.system_libs = ['setupapi','cfgmgr32','hid','advapi32','bcrypt']
        self.cpp_info.set_property("cmake_file_name", "NodeInterceptorSDK")
        self.cpp_info.set_property("cmake_target_name", "NodeInterceptorSDK::sdk")
