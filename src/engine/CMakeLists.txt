set(target engine)
add_library(${target})
target_compile_features(${target} PRIVATE cxx_std_26)
set_target_properties(${target} PROPERTIES FOLDER libs)
target_include_directories(${target} PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${target} PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)
target_link_libraries(${target} PRIVATE EnTT::EnTT)
target_sources(${target} PRIVATE
    engine.cpp
    entitymanager.cpp
)

###################################################################
# Engine Library
# file(GLOB_RECURSE ENGINE_SRC src/engine/*.cpp)
# add_library(engine ${ENGINE_SRC})
# target_link_libraries(engine PUBLIC sfml-system sfml-graphics sfml-window EnTT)