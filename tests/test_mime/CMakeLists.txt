cmake_minimum_required(VERSION 3.15)

project (unit_tests_mime)

find_package(Boost 1.71 REQUIRED COMPONENTS unit_test_framework)

add_executable(unit_tests_mime 
    main_test_mime.cpp
    test_address.cpp
    test_attachment.cpp
    test_charset.cpp
    test_message.cpp
    test_text.cpp
)

target_link_libraries(
    unit_tests_mime
PRIVATE
    Boost::unit_test_framework
    s2smtp::s2smtp
)

add_test (NAME test_mime COMMAND unit_tests_mime)