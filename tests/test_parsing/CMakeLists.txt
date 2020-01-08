
add_executable(unit_tests_parsing 
    main_test_parsing.cpp
    test_smtp_parse.cpp
)

target_link_libraries(
    unit_tests_parsing
PRIVATE
    Boost::unit_test_framework
    s2smtp::s2smtp
)

target_include_directories(
    unit_tests_parsing
PRIVATE
    ${s2smtp_SOURCE_DIR}/src
)

add_test (NAME test_parsing COMMAND unit_tests_parsing)