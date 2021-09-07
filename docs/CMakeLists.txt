
# 
# Target 'docs'
# 

if(NOT OPTION_BUILD_DOCS)
    return()
endif()

set(IDE_FOLDER "docs")  
add_custom_target(docs)

# 
# Project options
# 

set_target_properties(docs
    PROPERTIES
	    ${DEFAULT_PROJECT_OPTIONS}
	    FOLDER "${IDE_FOLDER}"
)

# 
# Documentation
# 

add_subdirectory(api-docs)
add_subdirectory(manual)
