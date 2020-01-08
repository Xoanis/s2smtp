
find_package(Boost 1.71 REQUIRED COMPONENTS unit_test_framework)

add_subdirectory(test_mime)
add_subdirectory(test_parsing)
add_subdirectory(test_serialization)
add_subdirectory(test_smtp)