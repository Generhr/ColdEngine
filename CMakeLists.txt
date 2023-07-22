cmake_minimum_required(VERSION 3.16...3.27 FATAL_ERROR) #? 3.16 - target_precompile_headers

#============  Safe Guards  ====================================================#

include(cmake/SafeGuards.cmake)

#==========  Project Details  ==================================================#

# Note: update this with your new project's details.

project("__Template"
        VERSION "0.1.0" # Note: Listing the version is nice here since it sets lots of useful variables.
        DESCRIPTION "C++ Template project"
        HOMEPAGE_URL "https://github.com/Onimuru/__Template"
        LANGUAGES CXX C # Note: To support external libraries, it's good practice to list both CXX and C.
)

#==========  CMake Variables  ==================================================#

message(STATUS "\n-- Configuring CMake for ${PROJECT_NAME} v${PROJECT_VERSION}...\n--")

# Only do these if this is the main project, and not if it is included through add_subdirectory:
if(PROJECT_IS_TOP_LEVEL)
    if(NOT DEFINED CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD # Note: GoogleTest requires at least C++14.
            20
            CACHE STRING "The C++ standard to use."
        )
    endif()

    # Require C++17 features like if constexpr:
    set(CMAKE_CXX_STANDARD_REQUIRED 17)

    set(CMAKE_CXX_EXTENSIONS OFF) # Note: While this is generally a good practice, you should ensure that this setting doesn't cause any issues with third-party libraries or the project's requirements.
    set(CMAKE_C_EXTENSIONS OFF)
endif()

# Suppress generation of the ZERO_CHECK build target:
set(CMAKE_SUPPRESS_REGENERATION true)

set(CMAKE_COLOR_DIAGNOSTICS ON)

#====  Define CMake Module Imports  ============================================#

include(cmake/Options.cmake)

include(cmake/UpdateSubmodules.cmake)

include(cmake/CCache.cmake)

if(ENABLE_DOXYGEN)
    include(cmake/Doxygen.cmake)
endif()

if(ENABLE_CODE_ANALYSIS)
    include(cmake/CodeAnalysis.cmake)
endif()

if(ENABLE_SANITIZE_ADDR OR ENABLE_SANITIZE_UNDEF)
    include(cmake/Sanitize.cmake)

    add_sanitizer_flags()
endif()

if(ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
endif()

include(cmake/Warnings.cmake)

if(USE_CONAN)
    message(STATUS "Using Conan")

    include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
elseif(USE_VCPKG)
    message(STATUS "Using VCPKG")

    set(VCPKG_BUILD_DOCS OFF)
    set(VCPKG_BUILD_TESTS OFF)
    set(VCPKG_BUILD_EXAMPLES OFF)

    if(VERBOSE_OUTPUT)
        set(VCPKG_VERBOSE ON)
    endif()

    option(VCPKG_APPLOCAL_DEPS "Automatically copy dependencies into the output directory for executables." ON)

    include(${CMAKE_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake)
endif()

#============== OS Fixes ======================================================#

# For Windows, it is necessary to link with the MultiThreaded library. Depending on how the rest of the project's
# dependencies are linked, it might be necessary to change the line to statically link with the library:
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

# This will allow the use of the same _DEBUG macro available in both Linux and Windows environment. Easy to put Debug specific code:
if(UNIX)
    add_compile_options("$<$<CONFIG:DEBUG>:-D_DEBUG>")
endif()

#==========  Configure Files  ==================================================#

add_subdirectory(configure)

#========== Add Source Files ==================================================#

add_subdirectory(external EXCLUDE_FROM_ALL)

set(PRECOMPILED_HEADER_LIST "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.

set(HEADER_LIST # Note: Add here any file you want to add manually which is not in include/*.
)

file(GLOB_RECURSE HEADER_FILES include/*${SUFFIX_HPPHEADER})

foreach(HEADER_FILE ${HEADER_FILES})
    if(NOT "${HEADER_FILE}" IN_LIST PRECOMPILED_HEADER_LIST) #~ https://gitlab.kitware.com/cmake/cmake/-/issues/22091
        list(APPEND HEADER_LIST ${HEADER_FILE})
    endif()
endforeach()

set(SOURCE_LIST # Note: Add here any file you want to add manually which is not in src/*.cpp.
)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

foreach(SOURCE_FILE ${SOURCE_FILES})
    if(NOT "${SOURCE_FILE}" MATCHES ".*/Main.cpp")
        list(APPEND SOURCE_LIST ${SOURCE_FILE})
    endif()
endforeach()

message(STATUS "Added all header and implementation files")

#===========  Create Target  ===================================================#

if(BUILD_HEADERS_ONLY)
    add_library(${PROJECT_NAME} INTERFACE)

    if(VERBOSE_OUTPUT)
        message(STATUS "Created ${PROJECT_NAME} interface library with the following headers:")

        foreach(HEADER IN LISTS HEADER_LIST)
            message(STATUS "* ${HEADER}")
        endforeach()
    else()
        message(STATUS "Created ${PROJECT_NAME} interface library")
    endif()
elseif(BUILD_EXECUTABLE)
    add_executable(${PROJECT_NAME} "src/Main.cpp" "${SOURCE_LIST}" "${HEADER_LIST}") # Note: Including header files here helps IDEs but is not required.

    if(VERBOSE_OUTPUT)
        message(STATUS "Created ${PROJECT_NAME} binary with the following sources:")
        message(STATUS "* ${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp")

        foreach(SOURCE IN LISTS SOURCE_LIST)
            message(STATUS "* ${SOURCE}")
        endforeach()
    else()
        message(STATUS "Created ${PROJECT_NAME} binary")
    endif()
else()
    add_library(${PROJECT_NAME} STATIC "${SOURCE_LIST}" "${HEADER_LIST}")

    if(VERBOSE_OUTPUT)
        message(STATUS "Created ${PROJECT_NAME} static library with the following sources:")

        foreach(SOURCE IN LISTS SOURCE_LIST)
            message(STATUS "* ${SOURCE}")
        endforeach()

        message(STATUS "and the following headers:")

        foreach(HEADER IN LISTS HEADER_LIST)
            message(STATUS "* ${HEADER}")
        endforeach()
    else()
        message(STATUS "Created ${PROJECT_NAME} static library")
    endif()
endif()

#=====  Set Standard And Warnings  =============================================#

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD}
               CXX_STANDARD_REQUIRED 17
               CXX_EXTENSIONS NO
               RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
               LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
               ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

message(STATUS "Using standard C++${CMAKE_CXX_STANDARD}")

if(ENABLE_WARNINGS)
    target_set_warnings(${PROJECT_NAME})
endif()

#========  Include Directories  ================================================#

if(BUILD_HEADERS_ONLY)
    target_precompile_headers(${PROJECT_NAME} INTERFACE "$<$<COMPILE_LANGUAGE:CXX>:${PRECOMPILED_HEADER_LIST}>")

    target_include_directories(${PROJECT_NAME} INTERFACE $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
else()
    target_precompile_headers(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${PRECOMPILED_HEADER_LIST}>")

    target_include_directories(
        ${PROJECT_NAME}
        PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
        PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_BINARY_DIR}/"
    )
endif()

#=========  Link Dependencies  =================================================#

find_package(cxxopts CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

if(BUILD_HEADERS_ONLY)
    target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json::nlohmann_json fmt::fmt spdlog::spdlog cxxopts::cxxopts)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json fmt::fmt spdlog::spdlog cxxopts::cxxopts)
endif()

#===========  Alias Targets  ===================================================#

if(BUILD_EXECUTABLE)
    add_executable(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
else()
    add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
endif()

#========= Unit Testing Setup =================================================#

if(PROJECT_IS_TOP_LEVEL)
    if(ENABLE_TESTING)
        if(ENABLE_CODE_COVERAGE)
            include(cmake/CodeCoverage.cmake)

            setup_target_for_coverage_lcov(
                NAME
                coverage
                EXECUTABLE
                ctest
                EXCLUDE
                "${CMAKE_CURRENT_SOURCE_DIR}/src/*"
                "${CMAKE_CURRENT_SOURCE_DIR}/test/*"
                "${CMAKE_CURRENT_SOURCE_DIR}/external/*"
                "${CMAKE_CURRENT_SOURCE_DIR}/vendor/*"
                "/*/.conan/*"
                "/usr/*"
                "/opt/*"
            )
        endif()

        add_library(TESTING_LIBRARY INTERFACE)

        target_precompile_headers(TESTING_LIBRARY REUSE_FROM ${PROJECT_NAME})

        set_target_properties(
            ${TESTING_LIBRARY}
            PROPERTIES CXX_STANDARD ${CMAKE_CXX_STANDARD}
                       CXX_STANDARD_REQUIRED 17
                       CXX_EXTENSIONS NO
        )

        target_sources(TESTING_LIBRARY INTERFACE "${SOURCE_LIST}" "${HEADER_LIST}") # Note: Including header files here helps IDEs but is not required.
        target_include_directories(TESTING_LIBRARY INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

        enable_testing()

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

#==============  Install  ======================================================#

include(GNUInstallDirs)

# Determine the installation directory for headers and libraries (the CMAKE_INSTALL_* variables are defined by the GNUInstallDirs module):
set(INSTALL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}-${PROJECT_VERSION}")
set(INSTALL_BIN_DIR "${CMAKE_INSTALL_BINDIR}")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_LIBDIR}")