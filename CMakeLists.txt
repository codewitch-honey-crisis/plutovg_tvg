cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(PLUTOVG_TVG_VERSION_MAJOR 0)
set(PLUTOVG_TVG_VERSION_MINOR 0)
set(PLUTOVG_TVG_VERSION_MICRO 1)

project(plutovg_tvg LANGUAGES C VERSION ${PLUTOVG_TVG_VERSION_MAJOR}.${PLUTOVG_TVG_VERSION_MINOR}.${PLUTOVG_TVG_VERSION_MICRO})

include(FetchContent)
FetchContent_Declare(plutovg
    GIT_REPOSITORY https://github.com/sammycage/plutovg.git
    GIT_TAG v0.0.3
    GIT_SHALLOW ON
    FIND_PACKAGE_ARGS 0.0.3
)

FetchContent_MakeAvailable(plutovg)

add_executable(plutovg_tvg 
    src/main.c
)

target_link_libraries(plutovg_tvg plutovg)
target_include_directories(plutovg_tvg PUBLIC
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/src"
  )
