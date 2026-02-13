set(target graphics)
add_library(${target})
target_sources(${target} PRIVATE
    gamescene.cpp
    scene.cpp
    scenemanager.cpp
    texturemanager.cpp
)
set_target_properties(${target} PROPERTIES FOLDER libs)
target_include_directories(${target} PRIVATE ${PROJECT_SOURCE_DIR}/src)
# target_include_directories(${target} PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${target} PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)
target_link_libraries(${target} PRIVATE EnTT::EnTT)
target_link_libraries(${target} PRIVATE engine)

###################################################################
#  Graphics Library
# file(GLOB_RECURSE GRAPHICS_SRC src/graphics/*.cpp)
# add_library(graphics ${GRAPHICS_SRC})
# target_link_libraries(graphics PUBLIC sfml-system sfml-graphics sfml-window EnTT)
# target_include_directories(graphics PUBLIC src/graphics/)
