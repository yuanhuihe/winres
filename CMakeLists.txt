# 
# CMake options
# 

# CMake version
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()
# 
# Project description and (meta) information
# 
set(GIT_HASH_ "unknown")
set(GIT_COMMIT_VER_ "0")
set(GIT_BRANCH_ "unknown")
find_package(Git)
if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    WORKING_DIRECTORY "${local_dir}"
    OUTPUT_VARIABLE GIT_HASH_
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  execute_process(
    COMMAND ${GIT_EXECUTABLE}  rev-list --count HEAD
    WORKING_DIRECTORY "${local_dir}"
    OUTPUT_VARIABLE GIT_COMMIT_VER_
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  execute_process(
    COMMAND ${GIT_EXECUTABLE}  name-rev --name-only HEAD
    WORKING_DIRECTORY "${local_dir}"
    OUTPUT_VARIABLE GIT_BRANCH_
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message( STATUS "GIT hash: ${GIT_HASH_}")
  message( STATUS "GIT rev: ${GIT_COMMIT_VER_}")
  message( STATUS "GIT branch: ${GIT_BRANCH_}")
else()
  message(STATUS "GIT not found")
endif()

# Set revision
set(GIT_REV "${GIT_COMMIT_VER_}")

# Meta information about the project
set(META_PROJECT_NAME        "winres")  		#winres-config.cmake
set(META_CORP_NAME           "Yuanhui.HE")
set(META_PROJECT_DESCRIPTION "Command line tool to modify the version number in resource file on Windows.")
set(META_AUTHOR_ORGANIZATION "Yuanhui")
set(META_AUTHOR_DOMAIN       "http://www.roboby.com")
set(META_AUTHOR_MAINTAINER   "hyh618@gmail.com")
set(META_VERSION_MAJOR       "1")
set(META_VERSION_MINOR       "0")
set(META_VERSION_PATCH       "0")
set(META_VERSION_REVISION    "${GIT_REV}")
set(META_VERSION_PRODUCTION  "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_VERSION             "${META_VERSION_PRODUCTION}.${META_VERSION_REVISION}")
set(META_NAME_VERSION        "${META_PROJECT_NAME} v${META_VERSION_PRODUCTION}, HASH: ${GIT_HASH_}")


# Include cmake modules

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(WriterCompilerDetectionHeaderFound NOTFOUND)

include(GenerateExportHeader)
include(ExternalProject)


# This module is only available with CMake >=3.1, so check whether it could be found
include(WriteCompilerDetectionHeader OPTIONAL RESULT_VARIABLE WriterCompilerDetectionHeaderFound)
include(cmake/Custom.cmake)
include(cmake/TargetArch.cmake)

# Set policies
set_policy(CMP0028 NEW) # ENABLE CMP0028: Double colon in target name means ALIAS or IMPORTED target.
set_policy(CMP0054 NEW) # ENABLE CMP0054: Only interpret if() arguments as variables or keywords when unquoted.
set_policy(CMP0042 NEW) # ENABLE CMP0042: MACOSX_RPATH is enabled by default.
set_policy(CMP0063 NEW) # ENABLE CMP0063: Honor visibility properties for all target types.
set_policy(CMP0058 NEW)

target_architecture(arch)


# 
# Project configuration options
# 

# Project options
option(BUILD_SHARED_LIBS     "Build shared instead of static libraries."              ON)
option(OPTION_SELF_CONTAINED "Create a self-contained install with all dependencies." ON)
option(OPTION_BUILD_TESTS    "Build tests."                                           OFF)
option(OPTION_BUILD_DOCS     "Build documentation."                                   ON)
option(OPTION_BUILD_DEMOS    "Build examples."                                        OFF)

#
#  Load libraries
#
option(OPTION_ISORNOT_LOAD "Is or not Load libraries" OFF)

# 
# Declare project
# 

# Generate folders for IDE targets (e.g., VisualStudio solutions)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Declare project
project(${META_PROJECT_NAME} C CXX)

# Disable C4828 for every project
add_compile_options(/wd"4100")
add_compile_options(/wd"4267")
add_compile_options(/wd"4828")
add_compile_options(/wd"4244")
add_compile_options(/wd"26444")

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

# Create version file
file(WRITE "${PROJECT_BINARY_DIR}/VERSION" "${META_NAME_VERSION}")

# Show version
message("-- Name Version: ${META_NAME_VERSION}")


# 
# Compiler settings and options
# 

include(cmake/CompileOptions.cmake)


# 
# Deployment/installation setup
# 

# Get project name
set(project ${META_PROJECT_NAME})

# Check for system dir install
set(SYSTEM_DIR_INSTALL FALSE)

if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr" OR "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
    set(SYSTEM_DIR_INSTALL TRUE)
endif()

# Installation paths
if(UNIX AND SYSTEM_DIR_INSTALL)
    # Install into the system (/usr/bin or /usr/local/bin)
    set(INSTALL_ROOT      "share/${project}")       # /usr/[local]/share/<project>
    set(INSTALL_CMAKE     "share/${project}/cmake") # /usr/[local]/share/<project>/cmake
    set(INSTALL_EXAMPLES  "share/${project}")       # /usr/[local]/share/<project>
    set(INSTALL_DATA      "share/${project}")       # /usr/[local]/share/<project>
    set(INSTALL_BIN       "bin")                    # /usr/[local]/bin
    set(INSTALL_SHARED    "lib")                    # /usr/[local]/lib
    set(INSTALL_LIB       "lib")                    # /usr/[local]/lib
    set(INSTALL_INCLUDE   "include")                # /usr/[local]/include
    set(INSTALL_DOC       "share/doc/${project}")   # /usr/[local]/share/doc/<project>
    set(INSTALL_SHORTCUTS "share/applications")     # /usr/[local]/share/applications
    set(INSTALL_ICONS     "share/pixmaps")          # /usr/[local]/share/pixmaps
    set(INSTALL_INIT      "/etc/init")              # /etc/init (upstart init scripts)
else()
    # Install into local directory
    set(INSTALL_ROOT      ".")                      # .
    set(INSTALL_CMAKE     "cmake")                  # ./cmake
    set(INSTALL_EXAMPLES  ".")                      # ./
    set(INSTALL_DATA      ".")                      # ./
    set(INSTALL_BIN       "bin")                    # ./bin
    set(INSTALL_SHARED    "share")                  # ./share
    set(INSTALL_LIB       "lib")                    # ./lib
    set(INSTALL_INCLUDE   "include")                # ./include
    set(INSTALL_DOC       "doc")                    # ./doc
    set(INSTALL_SHORTCUTS "misc")                   # ./misc
    set(INSTALL_ICONS     "misc")                   # ./misc
    set(INSTALL_INIT      "misc")                   # ./misc
endif()

# Set local install path
if(NOT ${SYSTEM_DIR_INSTALL} AND ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/Install")
endif()

# Set runtime path
set(CMAKE_SKIP_BUILD_RPATH            FALSE) # Add absolute path to all dependencies for BUILD
set(CMAKE_BUILD_WITH_INSTALL_RPATH    FALSE) # Use CMAKE_INSTALL_RPATH for INSTALL
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE) # Do NOT add path to dependencies for INSTALL

if(NOT SYSTEM_DIR_INSTALL)
    # Find libraries relative to binary
    if(APPLE)
        set(CMAKE_INSTALL_RPATH "@loader_path/../../../${INSTALL_LIB}")
    else()
        set(CMAKE_INSTALL_RPATH "$ORIGIN/${INSTALL_LIB}")       
    endif()
endif()

# 
# Project modules
# 
 
add_subdirectory(source)
add_subdirectory(docs)
add_subdirectory(deploy)


# 
# Deployment (global project files)
# 

# Install version file
install(FILES "${PROJECT_BINARY_DIR}/VERSION"               DESTINATION ${INSTALL_ROOT} COMPONENT runtime)

# Install cmake find script for the project
install(FILES ${META_PROJECT_NAME}-config.cmake	            DESTINATION ${INSTALL_CMAKE} COMPONENT dev)

# Install the project meta files
install(FILES AUTHORS                                       DESTINATION ${INSTALL_DOC} COMPONENT runtime)
install(FILES LICENSE                                       DESTINATION ${INSTALL_DOC} COMPONENT runtime)
install(FILES README.md                                     DESTINATION ${INSTALL_DOC} COMPONENT runtime)
