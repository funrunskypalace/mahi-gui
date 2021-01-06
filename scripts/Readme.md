# x64 与 x86
如果是x86编译,必须使用x86版本的CMake.exe,否则就会出现如下错误:
```
CMake Error at CMakeLists.txt:49 (find_package):
  Could not find a configuration file for package "spdlog" that is compatible
  with requested version "1.3.1".

  The following configuration files were considered but not accepted:

    d:/dev/vcpkg/installed/x86-windows/share/spdlog/spdlogConfig.cmake, version: 1.3.1 (32bit)
```
