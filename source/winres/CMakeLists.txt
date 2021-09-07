

# External dependencies
# 

# find_package(THIRDPARTY REQUIRED)

# Target name
set(target winres)

# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)


if(WIN32)
    # Date for filling in rc file information
    set(STAMP_FORMAT "%Y")
    unset(ENV{SOURCE_DATE_EPOCH})
    string(TIMESTAMP CURRENT_YEAR "${STAMP_FORMAT}")
    set(META_NAME ${target})
    configure_file(${target}.rc.in ${CMAKE_CURRENT_BINARY_DIR}/${target}.rc)
else(WIN32)
    configure_file(${target}.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME}/${META_PROJECT_NAME}-version.h)
endif(WIN32)

set(RES_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${target}.rc)
set(CMAKE_RC_COMPILER_INIT windres)
ENABLE_LANGUAGE(RC)
SET(CMAKE_RC_COMPILE_OBJECT 
"<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
set(TMAN_RC ${CMAKE_CURRENT_BINARY_DIR}/${target}.rc)

# Exit here if required dependencies are not met
message(STATUS "App: ${target}")

# 
# Sources
# 
file(GLOB_RECURSE headers
    "*.h"
    "*.hpp"
)

file(GLOB_RECURSE sources
    "*.c"
    "*.cpp"
)

# Group source files
set(header_group "Header Files")
set(source_group "Source Files")
source_group_by_path(${CMAKE_CURRENT_SOURCE_DIR} "\\\\.h$|\\\\.hpp$" 
    ${header_group} ${headers})
source_group_by_path(${CMAKE_CURRENT_SOURCE_DIR}  "\\\\.cpp$|\\\\.c$|\\\\.h$|\\\\.hpp$" 
    ${source_group} ${sources})


# 
# Create executable
# 

add_executable(${target} WIN32 ${sources} ${headers} ${UIS_HDRS} ${TMAN_QRC} ${TMAN_RC} ${LangSrcs} ${LangenSrcs})

# Create namespaced alias

add_executable(${META_PROJECT_NAME}::${target} ALIAS ${target})

set_target_properties(${target}
    PROPERTIES
        ${DEFAULT_PROJECT_OPTIONS}
		LINK_FLAGS /SUBSYSTEM:CONSOLE
        FOLDER "${IDE_FOLDER}"
)

add_definitions(-DUNICODE -D_UNICODE)

# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
        ${DEFAULT_INCLUDE_DIRECTORIES}
        ${PROJECT_BINARY_DIR}/source/include
		${CMAKE_CURRENT_SOURCE_DIR}/control
        ${PROJECT_SOURCE_DIR}/source
        ${PROJECT_SOURCE_DIR}/source/include
        ${PROJECT_SOURCE_DIR}/thirdparty
   )

# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE
        ${DEFAULT_LIBRARIES}
		strsafe
        )

# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE
        ${DEFAULT_COMPILE_DEFINITIONS}
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE
        ${DEFAULT_COMPILE_OPTIONS}
)

# Add requireAdministrator tag
#SET_TARGET_PROPERTIES(${target} PROPERTIES LINK_FLAGS "/level='requireAdministrator' /uiAccess='false'")

# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE
        ${DEFAULT_LINKER_OPTIONS}
)


# 
# Deployment
# 
# Executable
install(TARGETS ${target}
    RUNTIME DESTINATION ${INSTALL_BIN} COMPONENT runtime
    BUNDLE  DESTINATION ${INSTALL_BIN} COMPONENT runtime
)
