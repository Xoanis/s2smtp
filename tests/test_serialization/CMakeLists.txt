cmake_minimum_required(VERSION 3.15)

project (unit_tests_serialization)

find_package(Boost 1.71 REQUIRED COMPONENTS unit_test_framework)

add_executable(unit_tests_serialization 
    main_test_serialization.cpp
    test_address_serialization.cpp
    test_commands_serialization.cpp
    test_text_serialization.cpp
    test_message_serialization.cpp
)

target_link_libraries(
    unit_tests_serialization
PRIVATE
    Boost::unit_test_framework
    s2smtp::s2smtp
)

target_include_directories(
    unit_tests_serialization
PRIVATE
    ${s2smtp_SOURCE_DIR}/src
)

add_test (NAME test_serialization COMMAND unit_tests_serialization)