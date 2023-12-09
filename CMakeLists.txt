##########################
##   Subtask options    ##
##########################

# Change OFF to ON to enable
set(SUBTASK_CAST    OFF)
set(SUBTASK_WRAP    OFF)
set(SUBTASK_LITERAL OFF)
set(SUBTASK_CSV     OFF)
set(SUBTASK_XPATH   OFF)
set(SUBTASK_CONCAT  OFF)
set(SUBTASK_BINARY  OFF)

##########################

set(JSONCPP_NEWEST_VALIDATED_POLICIES_VERSION "3.13.2")
cmake_minimum_required(VERSION ${JSONCPP_NEWEST_VALIDATED_POLICIES_VERSION})
cmake_policy(VERSION ${JSONCPP_NEWEST_VALIDATED_POLICIES_VERSION})
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure that CMAKE_BUILD_TYPE has a value specified for single configuration generators.
if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Coverage.")
endif()

project(jsoncpp LANGUAGES CXX)

# Prevent in-source builds
get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)
if("${srcdir}" STREQUAL "${bindir}")
    message("jsoncpp should not be configured & built in the source directory")
    message("You must run cmake in a seperate build directory.")
    message(FATAL_ERROR "Quitting configuration")
endif()

option(JSONCPP_WITH_POST_BUILD_UNITTEST "Automatically run unit-tests as a post build step" ON)
option(JSONCPP_WITH_WARNING_AS_ERROR "Force compilation to fail if a warning occurs" OFF)
option(JSONCPP_WITH_STRICT_ISO "Issue all the warnings demanded by strict ISO C and ISO C++" ON)
option(JSONCPP_STATIC_WINDOWS_RUNTIME "Use static (MT/MTd) Windows runtime" OFF)

# Adhere to GNU filesystem layout conventions
include(GNUInstallDirs)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE PATH "Archive output dir.")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE PATH "Library output dir.")
set(CMAKE_PDB_OUTPUT_DIRECTORY     "${CMAKE_BINARY_DIR}/bin" CACHE PATH "PDB (MSVC debug symbol)output dir.")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE PATH "Executable/dll output dir.")

if(JSONCPP_WITH_WARNING_AS_ERROR)
    if(MSVC)
        # Only enabled in debug because some old versions of VS STL generate
        # warnings when compiled in release configuration.
        add_compile_options($<$<CONFIG:Debug>:/WX>)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-Werror)
        if(JSONCPP_WITH_STRICT_ISO)
            add_compile_options(-pedantic-errors)
        endif()
    endif()
endif()

# Include our configuration header
include_directories(${jsoncpp_SOURCE_DIR}/include)

if(MSVC)
    # Only enabled in debug because some old versions of VS STL generate
    # unreachable code warning when compiled in release configuration.
    add_compile_options($<$<CONFIG:Debug>:/W4>)
    if (JSONCPP_STATIC_WINDOWS_RUNTIME)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # using regular Clang or AppleClang
    add_compile_options(-Wall -Wconversion -Wshadow)

    if(JSONCPP_WITH_WARNING_AS_ERROR)
        add_compile_options(-Werror=conversion -Werror=sign-compare)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    add_compile_options(-Wall -Wconversion -Wshadow -Wextra)
    # not yet ready for -Wsign-conversion

    if(JSONCPP_WITH_STRICT_ISO)
        add_compile_options(-Wpedantic)
    endif()
    if(JSONCPP_WITH_WARNING_AS_ERROR)
        add_compile_options(-Werror=conversion)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    # using Intel compiler
    add_compile_options(-Wall -Wconversion -Wshadow -Wextra)

    if(JSONCPP_WITH_WARNING_AS_ERROR)
        add_compile_options(-Werror=conversion)
    elseif(JSONCPP_WITH_STRICT_ISO)
        add_compile_options(-Wpedantic)
    endif()
endif()

# Add googletest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://gitee.com/mirrors/googletest.git
    GIT_TAG        b10fad38c4026a29ea6561ab15fc4818170d1c10
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

# Build the different applications
add_subdirectory(src)

#install the example
    add_subdirectory(example)
