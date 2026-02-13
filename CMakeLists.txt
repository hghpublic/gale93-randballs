cmake_minimum_required(VERSION 4.2)

project(RANDBALLS LANGUAGES CXX)

# if using valgrind, set BUILD_WITH_ASAN to OFF
option(BUILD_WITH_ASAN "Build with ASAN" OFF)

find_package(EnTT CONFIG REQUIRED)
# target_link_libraries(${target} PRIVATE EnTT::EnTT)
find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
# target_link_libraries(${target} PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)
# target_link_libraries(${target} PRIVATE SFML::Main)

# libraries

add_subdirectory(src)

# randballs executable

set(target randballs)
add_executable(${target})
target_compile_features(${target} PRIVATE cxx_std_26)
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${target} PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)
target_link_libraries(${target} PRIVATE EnTT::EnTT)
target_link_libraries(${target} PRIVATE engine graphics systems)
target_sources(${target} PRIVATE
    src/main.cpp
)
if(BUILD_WITH_ASAN)
    target_compile_options(${target} PRIVATE -fsanitize=address)
    target_link_options(${target} PRIVATE -fsanitize=address)
endif()

# copy media

add_custom_command(TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
        ${CMAKE_CURRENT_SOURCE_DIR}/media
        $<TARGET_FILE_DIR:${target}>/media)


# valgrind target

add_custom_target(valgrind
    COMMAND valgrind
        --leak-check=full
        --show-leak-kinds=all
        --track-origins=yes
        -s --suppressions=$ENV{DEV_ROOT}/valgrind/randballs.valgrind.supp
        --gen-suppressions=all
        --log-file=$ENV{DEV_ROOT}/valgrind/logs/memcheck.log
        $<TARGET_FILE:${target}>
    DEPENDS ${target}
    WORKING_DIRECTORY $<TARGET_FILE_DIR:${target}>
    COMMENT "Running randballs with valgrind"
)