cmake_minimum_required(VERSION 3.15)

project (unit_tests_smtp)

find_package(Boost 1.71 REQUIRED COMPONENTS unit_test_framework)

add_executable(unit_tests_smtp 
    main_test_smtp.cpp
    test_commands.cpp
    test_server_reply.cpp
)

target_link_libraries(
    unit_tests_smtp
PRIVATE
    Boost::unit_test_framework
    s2smtp::s2smtp
)

target_include_directories(
    unit_tests_smtp
PRIVATE
    ${s2smtp_SOURCE_DIR}/src
)

add_test (NAME test_smtp COMMAND unit_tests_smtp)