
set(target components)
add_library(${target} INTERFACE)
target_include_directories(${target} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set(target events)
add_library(${target} INTERFACE)
target_include_directories(${target} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(engine)
add_subdirectory(graphics)
add_subdirectory(systems)