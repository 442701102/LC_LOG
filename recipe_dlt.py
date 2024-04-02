from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout
from conan.tools.files import get
import os

class DltRecipe(ConanFile):
    name = "dlt-daemon"
    version = "2.18.10"
    license = "MPL-2.0"  # Mozilla Public License version 2.0
    generators = "CMakeDeps"

    author = "Alexander Wenzel (alexander.aw.wenzel@bmw.de)"
    url = "https://github.com/COVESA/dlt-daemon"
    description = ("COVESA DLT provides a log and trace interface, based on the standardised protocol "
                   "specified in the AUTOSAR standard 4 DLT. It is used by other COVESA components but "
                   "can serve as logging framework for other applications without relation to COVESA.")
    topics = ("AUTOSAR", "DLT", "logging", "tracing")
    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    def source(self):
        # Please, be aware that using the head of the branch instead of an immutable tag
        # or commit is a bad practice and not allowed by Conan
        get(self, "https://github.com/COVESA/dlt-daemon/archive/refs/tags/v2.18.10.zip",
                  strip_root=True)
        cmake_file_path = os.path.join(self.source_folder, "CMakeLists.txt")
        if os.path.exists(cmake_file_path):
            print("CMakeLists.txt exists: Yes")
            # 这里可以放置使用CMake构建的逻辑
        else:
            print("CMakeLists.txt exists: No")
            # 这里可以放置使用其他构建系统的逻辑

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake_file_path = os.path.join(self.source_folder, "CMakeLists.txt")
        if os.path.exists(cmake_file_path):
            print("build CMakeLists.txt exists: Yes")
            # 这里可以放置使用CMake构建的逻辑
        else:
            print("build CMakeLists.txt exists: No")
            # 这里可以放置使用其他构建系统的逻辑
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["dlt"]