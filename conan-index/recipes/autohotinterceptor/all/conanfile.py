from conan.tools.files import get
from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, cmake_layout
class NativeSDK(ConanFile):
    name = 'autohotinterceptor'
    version = "1.0.0"
    license = "MIT"
    description = "Public Windows x64 user-mode SDK for AutoHotInterceptor; driver not included."
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
    def package_info(self):
        self.cpp_info.defines = ["INTERCEPTION_STATIC"]
        self.cpp_info.libs = ["TitanAutoHotInterceptor"]
        self.cpp_info.includedirs = ['include', 'include/Native', 'include/Native/SDK']
        self.cpp_info.system_libs = ['advapi32', 'bcrypt', 'cfgmgr32', 'comctl32', 'hid', 'newdev', 'ole32', 'setupapi', 'user32']
        self.cpp_info.set_property("cmake_file_name", "AutoHotInterceptorSDK")
        self.cpp_info.set_property("cmake_target_name", "AutoHotInterceptorSDK::sdk")
