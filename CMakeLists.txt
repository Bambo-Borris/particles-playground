cmake_minimum_required(VERSION 3.16)
project(particle-playground CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external)
add_subdirectory(apputils)

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /WX /permissive-)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(particle-playground WIN32)
    target_link_libraries(particle-playground PRIVATE SFML::Main)
else()
    add_executable(particle-playground)
endif()

# if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#     target_sources(particle-playground PRIVATE src/appicon.rc)
# endif()

target_compile_features(particle-playground PUBLIC cxx_std_17)
target_include_directories(particle-playground PUBLIC volkanic-toolkit/include)

target_sources(particle-playground PRIVATE
    src/main.cpp
    )
target_link_libraries(particle-playground PRIVATE SFML::Graphics ImGui-SFML::ImGui-SFML spdlog apputils)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(particle-playground PRIVATE ONE_HIT_WONDER_DEBUG)
endif()

add_custom_target(format
    COMMAND clang-format -i `git ls-files *.hpp *.cpp`
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
add_custom_target(run COMMAND particle-playground WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
