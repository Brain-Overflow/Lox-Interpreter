cmake_minimum_required(VERSION 3.20)

project(Lox
    VERSION 0.1.0
    DESCRIPTION "A tree-walking interpreter for Lox (without classes)"
    LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# Language standard
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for clangd / editor tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to a Debug build if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(LOX_ENABLE_SANITIZERS "Build with AddressSanitizer and UBSan" OFF)

# ---------------------------------------------------------------------------
# Sources
# All .cpp files under src/ are compiled. CONFIGURE_DEPENDS makes CMake
# re-check the folder on each build, so new files are picked up automatically.
# ---------------------------------------------------------------------------
file(GLOB_RECURSE LOX_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(Lox ${LOX_SOURCES})

target_include_directories(Lox PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Put the binary at the top of the build directory so it runs as ./Lox
set_target_properties(Lox PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# ---------------------------------------------------------------------------
# Warnings
# ---------------------------------------------------------------------------
if(MSVC)
    target_compile_options(Lox PRIVATE /W4 /permissive-)
else()
    target_compile_options(Lox PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wnon-virtual-dtor
        -Wold-style-cast
    )
endif()

# ---------------------------------------------------------------------------
# Sanitizers (GCC/Clang only)
# ---------------------------------------------------------------------------
if(LOX_ENABLE_SANITIZERS AND NOT MSVC)
    target_compile_options(Lox PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(Lox PRIVATE -fsanitize=address,undefined)
endif()