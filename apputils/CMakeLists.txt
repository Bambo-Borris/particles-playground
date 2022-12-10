add_library(apputils STATIC
src/apputils/AssetHolder.cpp
src/apputils/InputHandler.cpp
src/apputils/RNG.cpp)

target_compile_features(apputils PUBLIC cxx_std_17)
target_include_directories(apputils PUBLIC include)
target_link_libraries(apputils PRIVATE SFML::Graphics spdlog)