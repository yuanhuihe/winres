
# 
# Default dependencies for the runtime-package
# 
if( ${arch} STREQUAL "x86_64")
set(build_arch "x64")
elseif(${arch} STREQUAL "i386")
set(build_arch "x86") 
endif()

# Install 3rd-party runtime dependencies into runtime-component
# install(FILES ... COMPONENT runtime)

#install(FILES ${PROJECT_SOURCE_DIR}/thirdparty/xxx/xxx/${build_arch}/xxx.dll                        DESTINATION ${INSTALL_BIN} COMPONENT runtime)
if(OPTION_ISORNOT_LOAD)


endif()

# 
# Tools
# 


# 
# Language package
# 
install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/langs                                                  DESTINATION ${INSTALL_BIN}/plugins COMPONENT runtime)

# 
# Full dependencies for self-contained packages
# 

if(OPTION_SELF_CONTAINED)

    # Install 3rd-party runtime dependencies into runtime-component
    # install(FILES ... COMPONENT runtime)

endif()
