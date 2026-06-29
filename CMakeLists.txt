cmake_minimum_required(VERSION 3.28)
project(trees LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FETCHContent_Declare(StableIndexVector
    GIT_REPOSITORY https://github.com/johnBuffer/StableIndexVector.git
    GIT_TAG main)
FetchContent_MakeAvailable(SFML StableIndexVector)

add_executable(trees src/main.cpp)
target_sources(trees
PRIVATE
    src/simulation.cpp
    src/events.cpp
    src/render.cpp
)
target_include_directories(trees SYSTEM PRIVATE
  "${FETCHCONTENT_BASE_DIR}/stableindexvector-src/"
)
target_compile_features(trees PRIVATE cxx_std_17)
target_link_libraries(trees PRIVATE SFML::Graphics)
target_compile_options(trees PRIVATE -O3)