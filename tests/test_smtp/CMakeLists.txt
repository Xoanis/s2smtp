
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