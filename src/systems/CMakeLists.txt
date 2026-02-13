set(target systems)
add_library(${target})
target_sources(${target} PRIVATE
    basesystem.cpp
    collisionsystem.cpp
    colorsystem.cpp
    movesystem.cpp
    rendersystem.cpp
    teleportsystem.cpp
    usersystem.cpp
)
set_target_properties(${target} PROPERTIES FOLDER libs)
target_include_directories(${target} PRIVATE ${PROJECT_SOURCE_DIR}/src)
# target_include_directories(${target} PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${target} PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)
target_link_libraries(${target} PRIVATE EnTT::EnTT)

###################################################################
#  Systems Library
# file(GLOB_RECURSE SYSTEMS_SRC src/systems/*.cpp)
# add_library(systems ${SYSTEMS_SRC})
# target_link_libraries(systems PUBLIC sfml-system sfml-graphics sfml-window EnTT)
# target_include_directories(systems PUBLIC src/graphics/)
