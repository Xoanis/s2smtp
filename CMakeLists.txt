cmake_minimum_required(VERSION 3.15)
project(s2smtp VERSION 0.1.0 LANGUAGES CXX)

OPTION( BUILD_EXAMPLES "build examples?" ON )

find_package(Boost 1.71 COMPONENTS thread system date_time REQUIRED)
find_package(OpenSSL REQUIRED)

list(INSERT CMAKE_MODULE_PATH 0 ${s2smtp_SOURCE_DIR}/cmake)
find_package(cppcodec REQUIRED MODULE)

add_library(s2smtp
    src/mime/address.cpp
    src/mime/attachment.cpp
    src/mime/charset.cpp
    src/mime/text.cpp
    src/mime/message.cpp
    src/network/channel.cpp
    src/parsing/smtp_parse.cpp
    src/serialization/mime/address_serialization.cpp
    src/serialization/mime/header_serialization.cpp
    src/serialization/mime/message_serialization.cpp
    src/serialization/mime/text_serialization.cpp
    src/serialization/smtp/commands_serialization.cpp
    src/smtp/server_reply.cpp
    src/smtp/commands.cpp
    src/smtp/sender.cpp
    src/smtp/client.cpp
)

include(GenerateExportHeader)
generate_export_header(s2smtp EXPORT_FILE_NAME s2smtp/s2smtp_export.h)

add_library(s2smtp::s2smtp ALIAS s2smtp)

target_compile_features(s2smtp PUBLIC cxx_std_17 )

target_include_directories(
    s2smtp 
PUBLIC      
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
    s2smtp 
PRIVATE
    Boost::headers
    Boost::thread
    Boost::system
    Boost::date_time
    OpenSSL::SSL
    cppcodec::cppcodec
    stdc++fs
)
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8)
        message(FATAL_ERROR "GCC version must be >= 8!")
    endif()
    
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
        message(STATUS "GCC version is less than 9. Linking stdc++fs")
        target_link_libraries(
            s2smtp 
        PRIVATE
            stdc++fs
        )
    endif()
endif()


include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/s2smtp)

configure_file(s2smtpConfig.h.in s2smtp/s2smtpConfig.h)

install(TARGETS s2smtp EXPORT s2smtpTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT s2smtpTargets
    FILE s2smtpTargets.cmake
    NAMESPACE s2smtp::
    DESTINATION ${INSTALL_CONFIGDIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/s2smtpConfigVersion.cmake
    VERSION ${s2smtp_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/s2smtpConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/s2smtpConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
    ${CMAKE_CURRENT_LIST_DIR}/cmake/Findcppcodec.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/s2smtpConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/s2smtpConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/s2smtp/s2smtp_export.h
    ${CMAKE_CURRENT_BINARY_DIR}/s2smtp/s2smtpConfig.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/s2smtp/
)

configure_file(${CMAKE_CURRENT_LIST_DIR}/cmake/Findcppcodec.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/Findcppcodec.cmake
    COPYONLY)

export(EXPORT s2smtpTargets FILE ${CMAKE_CURRENT_BINARY_DIR}/s2smtpTargets.cmake NAMESPACE s2smtp::)

export(PACKAGE s2smtp)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

include( CTest )
enable_testing()

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()






