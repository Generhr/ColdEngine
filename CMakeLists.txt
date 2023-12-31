cmake_minimum_required(VERSION 3.27...3.27 FATAL_ERROR) #? 3.27 - SKIP_LINTING

#==========  Project Details  ==================================================#

project("ColdEngine"
        VERSION "1.6.1"
        DESCRIPTION "Direct3D Wrapper / 3D Engine written in C++"
        HOMEPAGE_URL "https://github.com/Onimuru/ColdEngine"
        LANGUAGES CXX C
)

#============  Safe Guards  ====================================================#

include(cmake/SafeGuards.cmake)
variable_watch(CMAKE_BUILD_TYPE readonly_guard)

#==========  CMake Variables  ==================================================#

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang*")
    if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC") # NOTE: `CMAKE_CXX_COMPILER_FRONTEND_VARIANT` requires CMake v3.14+
        set(CMAKE_COMPILER_IN_USE "Clang-cl")
    elseif(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        set(CMAKE_COMPILER_IN_USE "Clang")
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_COMPILER_IN_USE "GNU")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_COMPILER_IN_USE "MSVC")
endif()

message(STATUS "\n-- Configuring CMake for ${PROJECT_NAME} v${PROJECT_VERSION}\n-- with ${CMAKE_COMPILER_IN_USE} compiler\n")

if(PROJECT_IS_TOP_LEVEL)
    if(NOT DEFINED CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD
            20
            CACHE STRING "The C++ standard to use."
        )
    endif()

    set(CMAKE_CXX_STANDARD_REQUIRED 17)

    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_C_EXTENSIONS OFF)
endif()

set(CMAKE_SUPPRESS_REGENERATION ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_COLOR_DIAGNOSTICS ON)

include(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT)

if(PROCESSOR_COUNT EQUAL 0)
    set(PROCESSOR_COUNT 1)
endif()

string(TOLOWER "${PROJECT_NAME}" PROJECT_NAME_TOLOWER)
string(TOUPPER "${PROJECT_NAME}" PROJECT_NAME_TOUPPER)

set(GIT_SHA
    "Unknown"
    CACHE STRING "SHA this build was generated from"
)
string(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORT_SHA)

#=======  Manual File Additions  ===============================================#

set(SKIP_LINTING_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/DXErr.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/DXErr.h")

set(PRECOMPILED_HEADER_LIST "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")

set(MANUAL_HEADER_LIST)

set(MANUAL_SOURCE_LIST)

#====  Define CMake Module Imports  ============================================#

include(CMakeParseArguments)

include(cmake/Options.cmake)

include(cmake/Utilities.cmake)

include(cmake/UpdateSubmodules.cmake)

include(cmake/Cache.cmake)
enable_cache()

if(ENABLE_STATIC_ANALYZERS)
    include(cmake/StaticAnalyzers.cmake)

    if(ENABLE_CPPCHECK)
        enable_cppcheck()
    endif()

    if(ENABLE_CLANG_TIDY)
        enable_clang_tidy()
    endif()

    if(ENABLE_INCLUDE_WHAT_YOU_USE)
        enable_include_what_you_use()
    endif()
endif()

if(ENABLE_SANITIZE_ADDR OR ENABLE_SANITIZE_UNDEF)
    include(cmake/Sanitize.cmake)
    add_sanitizer_flags()
endif()

if(ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    enable_ipo()
endif()

include(cmake/Warnings.cmake)

set(VCPKG_BUILD_DOCS OFF)
set(VCPKG_BUILD_TESTS OFF)
set(VCPKG_BUILD_EXAMPLES OFF)

if(VERBOSE_OUTPUT)
    set(VCPKG_VERBOSE ON)
else()
    list(APPEND VCPKG_INSTALL_OPTIONS "--no-print-usage") #: https://learn.microsoft.com/en-us/vcpkg/commands/install
endif()

option(VCPKG_APPLOCAL_DEPS "Automatically copy dependencies into the output directory for executables." ON)

include("${CMAKE_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake")

#=========  Compiler/OS Fixes  =================================================#

if(CMAKE_COMPILER_IN_USE STREQUAL "Clang")
    if(WIN32)
        add_compile_options($<$<COMPILE_LANGUAGE:C>:-fcolor-diagnostics> $<$<COMPILE_LANGUAGE:CXX>:-fcolor-diagnostics>)
    else()
        add_compile_options(-fcolor-diagnostics)
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLLVM_USE_LINKER=lld -DLLVM_CCACHE_BUILD=true")

    set(PROJECT_INCLUDE_DIRECTORIES "$ENV{EXTERNAL_INCLUDE}")
elseif(CMAKE_COMPILER_IN_USE STREQUAL "GNU")
    if(WIN32)
        add_compile_options($<$<COMPILE_LANGUAGE:C>:-fdiagnostics-color=always> $<$<COMPILE_LANGUAGE:CXX>:-fdiagnostics-color=always>)
    else()
        add_compile_options(-fdiagnostics-color=always)
    endif()
elseif(CMAKE_COMPILER_IN_USE STREQUAL "Clang-cl" OR CMAKE_COMPILER_IN_USE STREQUAL "MSVC")
    if(MSVC_VERSION GREATER 1900)
        add_compile_options(/diagnostics:column)
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
else()
    message(STATUS "No colored compiler diagnostic set for '${CMAKE_COMPILER_IN_USE}' compiler.")
endif()

#==========  Configure Files  ==================================================#

configure_file("configure/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/configured_files/include/config.h" ESCAPE_QUOTES)

if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/bin/ShaderOutput/FramebufferPS.cso" OR NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/bin/ShaderOutput/FramebufferVS.cso")
    # List of HLSL shader source files
    set(HLSL_SHADER_FILES shaders/FramebufferPS.hlsl shaders/FramebufferVS.hlsl)

    # Iterate through each shader and compile it
    foreach(SHADER ${HLSL_SHADER_FILES})
        get_filename_component(SHADER_NAME ${SHADER} NAME_WE)
        set(CSO_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bin/ShaderOutput/${SHADER_NAME})

        string(REGEX REPLACE ".*(PS|VS)\\.hlsl" "\\1_4_0_level_9_1" TARGET_PROFILE ${SHADER})
        string(TOLOWER ${TARGET_PROFILE} TARGET_PROFILE)

        add_custom_command(
            OUTPUT ${CSO_OUTPUT}
            COMMAND fxc /T ${TARGET_PROFILE} # Target profile
                    /E main # Entry point function in the shader
                    $<IF:$<CONFIG:DEBUG>,/Od,/O1> # Optimization level
                    -Zi # Enable debugging information
                    -WX # Treat warnings as errors
                    #/Vn Data # Variable name in header file
                    #/Fh ${CSO_OUTPUT}.shh  # Output header file containing object code
                    /Fo ${CSO_OUTPUT}.cso # Output object file
                    /nologo # Suppress copyright message
                    ${CMAKE_SOURCE_DIR}/${SHADER}
            MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/${SHADER}
            COMMENT "Compiling ${SHADER_NAME}.hlsl"
        )

        list(APPEND CSO_FILES ${CSO_OUTPUT})
    endforeach()

    list(APPEND MANUAL_SOURCE_LIST "${CSO_FILES}") # NOTE: Adding `CSO_FILES` here ensures that they are built if the variable has been set and that only happens if the .cso files have been deleted
endif()

# Move textures into place so that they are relative to the executable
file(GLOB_RECURSE COPY_FILES "res/*")
file(COPY ${COPY_FILES} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/res")

#========== Add Source Files ==================================================#

add_subdirectory(external EXCLUDE_FROM_ALL)

file(GLOB_RECURSE HEADER_LIST include/*)
list(APPEND HEADER_LIST ${MANUAL_HEADER_LIST})
list(REMOVE_DUPLICATES HEADER_LIST)
list(REMOVE_ITEM HEADER_LIST ${PRECOMPILED_HEADER_LIST})

file(GLOB_RECURSE SOURCE_LIST src/*.cpp)
list(APPEND SOURCE_LIST ${MANUAL_SOURCE_LIST})
list(REMOVE_DUPLICATES SOURCE_LIST)
list(REMOVE_ITEM SOURCE_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp")

foreach(SOURCE_FILE IN LISTS SKIP_LINTING_LIST)
    set_source_files_properties(${SOURCE_FILE} PROPERTIES SKIP_LINTING ON)
endforeach()

list(APPEND PROJECT_INCLUDE_DIRECTORIES $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

#===========  Create Target  ===================================================#

# Main executable
add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp" "${SOURCE_LIST}" "${HEADER_LIST}")

if(VERBOSE_OUTPUT)
    message(STATUS "Created ${PROJECT_NAME} binary with the following sources:")
    message(STATUS "* ${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp")

    foreach(SOURCE IN LISTS SOURCE_LIST)
        message(STATUS "* ${SOURCE}")
    endforeach()
else()
    message(STATUS "Created ${PROJECT_NAME} binary")
endif()

# Inlcude
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC "${PROJECT_INCLUDE_DIRECTORIES}"
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/configured_files/include"
)

# Precompile headers
if(NOT ENABLE_CLANG_TIDY OR CMAKE_COMPILER_IN_USE STREQUAL "Clang")
    target_precompile_headers(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${PRECOMPILED_HEADER_LIST}>")
endif()

#=====  Set Standard and Warnings  =============================================#

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD}
               CXX_STANDARD_REQUIRED 17
               CXX_EXTENSIONS NO
               RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
               LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
               ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
)

message(STATUS "* using standard C++${CMAKE_CXX_STANDARD}")

if(ENABLE_WARNINGS)
    target_set_warnings(${PROJECT_NAME})
endif()

#========  Compile Definitions  ================================================#

# Target
target_compile_definitions(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:-D${PROJECT_NAME_TOUPPER}_DEBUG>")

#=========  Link Dependencies  =================================================#

#===========  Alias Targets  ===================================================#

add_executable(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

#==============  Install  ======================================================#

include(GNUInstallDirs)

# Determine the installation directory for headers and libraries (the CMAKE_INSTALL_* variables are defined by the GNUInstallDirs module)
set(INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${PROJECT_VERSION}")
set(INSTALL_BIN_DIR "${CMAKE_INSTALL_BINDIR}")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_LIBDIR}")

#========= Unit Testing Setup =================================================#

if(PROJECT_IS_TOP_LEVEL)
    if(ENABLE_TESTING)
        set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
        include(CTest)

        add_library(TESTING_LIBRARY INTERFACE)

        if(ENABLE_CODE_COVERAGE)
            include(cmake/CodeCoverage.cmake)
            append_coverage_compiler_flags_to_target(TESTING_LIBRARY)
        endif()

        target_sources(TESTING_LIBRARY INTERFACE "${SOURCE_LIST}" "${HEADER_LIST}") # NOTE: Including header files here helps IDEs but is not required
        target_include_directories(TESTING_LIBRARY INTERFACE "${PROJECT_INCLUDE_DIRECTORIES}")

        if(NOT ENABLE_CLANG_TIDY OR CMAKE_COMPILER_IN_USE STREQUAL "Clang")
            target_precompile_headers(TESTING_LIBRARY INTERFACE "$<$<COMPILE_LANGUAGE:CXX>:${PRECOMPILED_HEADER_LIST}>")
        endif()

        target_compile_definitions(TESTING_LIBRARY INTERFACE "${PROJECT_NAME_TOUPPER}_TEST")

        find_package(GTest CONFIG REQUIRED)
        include(GoogleTest)

        mark_as_advanced(
            BUILD_GMOCK
            BUILD_GTEST
            BUILD_SHARED_LIBS
            gmock_build_tests
            gtest_build_samples
            gtest_build_tests
            gtest_disable_pthreads
            gtest_force_shared_crt
            gtest_hide_internal_symbols
        )

        add_subdirectory(test)
    endif()
endif()

#==============  Doxygen  ======================================================#

if(ENABLE_DOXYGEN)
    include(cmake/Doxygen.cmake)
    enable_doxygen()
endif()
