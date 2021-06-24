cmake_minimum_required(VERSION 3.9)
project(reese)

set(CMAKE_C_STANDARD 99)

add_compile_definitions(REESE_EXPORTS)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(${PROJECT_NAME} SHARED src/reese_core.c)

target_include_directories(${PROJECT_NAME}
                           PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>
                           PRIVATE src)

install(TARGETS ${PROJECT_NAME} EXPORT ReeseConfig
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(EXPORT ReeseConfig DESTINATION share/Reese/cmake)

export(TARGETS ${PROJECT_NAME} FILE ReeseConfig.cmake)