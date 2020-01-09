[![Action Status](https://github.com/Xoanis/s2smtp/workflows/build/badge.svg)]
[![BCH compliance](https://bettercodehub.com/edge/badge/Xoanis/s2smtp?branch=master)](https://bettercodehub.com/)
# s2smtp #

s2smtp is a cross platform SMTP client based on boost and c++17.

# Features #

- RFC compliant;
- auth login mechanism;
- SSL/TLS, STARTTLS;
- differents inline body content-types (plain, html, etc ...);
- non-ascii characters;
- attachments;
- easy to use;

# Usage example: #

```cpp
#include "s2smtp/s2smtp.hpp"
#include <iostream>

int main(int, char **) {
  try {
    using namespace s2smtp;
    credentials_t cr{"my_email@gmail.com", "application password"};
    mime::message_t msg(cr.user);
    msg.header.to = { mime::mailbox_t{"user_1@mail.com", "User"} };
    msg.header.cc = { mime::mailbox_t{"user_2@gmail.com"} };
    msg.header.subject = "hello";
    msg.text = "inline message";
    msg.attachments.emplace_back("D:/1.png");
    end_point_t ep{"smtp.gmail.com", 589};
    client_t(ep, security_method_t::e_ssl_tls, cr).submit(msg);
    std::cout << "transferred\n";
  } catch (s2smtp::error &e) {
    std::cerr << "smtp error: " << e.what() << '\n';
  } catch (std::exception &e) {
    std::cerr << "exception: " << e.what() << '\n';
  } catch (...) {
    std::cerr << "unknown error\n";
  }
}
```

# Requirements #
CMake 3.15

Windows: Visual Studio 15 2017

Linux: g++-9

Dependency:
- boost 1.71 
  boost components:
  - asio (OpenSSL is required)
  - spirit (x3)
  - unit_test_framework (optional, used to compile tests)
  - program_options (optional, used to compile example)
- cppcodec

# Building #

Windows:

The easiest  way is using vcpkg:

```ps1
mkdir example
cd example
git clone https://github.com/microsoft/vcpkg
cd .\vcpkg\
.\bootstrap-vcpkg.bat
.\vcpkg.exe integrate install
```
Copy text "-DCMAKE_TOOLCHAIN_FILE=some-path/vcpkg/scripts/buildsystems/vcpkg.cmake"

```ps1
.\vcpkg.exe install boost-asio boost-spirit boost-program-options boost-test cppcodec
cd ..
git clone https://github.com/Xoanis/s2smtp.git
cd s2smtp
mkdir build
cd build
cmake .. "-DCMAKE_TOOLCHAIN_FILE=some-path/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build .
```
If you use CMake to build your project and you want to link s2smtp with find_package(s2smtp) command you need to install s2smtp package:
```ps1
cmake --install .
```
And your CMakeLists.txt would look like:
```cmake
...
find_package(s2smtp) 
add_executable(${PROJECT_NAME} main.cpp ...)
...
target_link_libraries(${PROJECT_NAME} PRIVATE s2smtp::s2smtp)
...
```

Linux:

Building on Linux can be done in the same way.


