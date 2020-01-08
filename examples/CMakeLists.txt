cmake_minimum_required(VERSION 3.15)
project(example)

find_package(Boost COMPONENTS program_options REQUIRED)

add_executable(${PROJECT_NAME} example.cpp)

target_link_libraries(
    ${PROJECT_NAME}
PRIVATE
    s2smtp::s2smtp
    Boost::program_options
)
