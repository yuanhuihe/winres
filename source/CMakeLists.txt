
#
# Configuration for all sub-projects
#

# Generate version-header
string(TOUPPER ${META_PROJECT_NAME} META_PROJECT_NAME_UPPER)
configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME}/${META_PROJECT_NAME}-version.h)

# Generate version-rc for Win32
if(WIN32)
    # Date for filling in rc file information
    set(STAMP_FORMAT "%Y")
    unset(ENV{SOURCE_DATE_EPOCH})
    string(TIMESTAMP CURRENT_YEAR "${STAMP_FORMAT}")
    set(META_NAME "@META_NAME@")
    configure_file(version.rc.in ${PROJECT_BINARY_DIR}/version.rc)
endif(WIN32)

#
# Sub-projects
#

# Libraries
set(IDE_FOLDER "DLL")

# App
set(IDE_FOLDER "EXE")
add_subdirectory(winres)

# Examples
set(IDE_FOLDER "Demo")
#add_subdirectory(demo)

# Tests
set(IDE_FOLDER "Tests")
#add_subdirectory(tests)


#
# Deployment
#

# Deploy generated headers
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/${META_PROJECT_NAME} DESTINATION include COMPONENT dev)
