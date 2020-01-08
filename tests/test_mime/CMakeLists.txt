
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