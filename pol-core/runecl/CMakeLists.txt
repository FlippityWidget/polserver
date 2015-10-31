###############################################################################

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/../../cmake/CMakeConfig.txt)

MESSAGE("####################################")
MESSAGE("##### Building RUNECL")
MESSAGE("####################################")

###############################################################################
# define source files
SET (SOURCES  # sorted !
    ../pol/binaryfilescrobj.cpp 
    ../pol/cfgrepos.cpp 
    ../pol/dice.cpp
    ../pol/globals/ucfg.cpp
    ../pol/module/basiciomod.cpp 
    ../pol/module/basicmod.cpp 
    ../pol/module/cfgmod.cpp 
    ../pol/module/datastore.cpp
    ../pol/module/filemod.cpp 
    ../pol/module/mathmod.cpp
    ../pol/module/utilmod.cpp 
    ../pol/proplist.cpp
    ../pol/xmlfilescrobj.cpp
    runecl.cpp
)    

##############################################################
# include dirs
SET (INCLUDE_DIRS
    .
    ../
)

##############################################################
# set directory for internal libs
SET (LIB_DIRS
    ${LIB_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/../bin
)
    
##############################################################
# link to the needed libs
SET (LIBS_LINUX
    bscript
    clib_debugging
    clib
    plib
    tinyxml
)
SET (LIBS_LINUX_STATIC_BIN_STATIC
    bscript_static
    clib_debugging_static
    clib_static
    plib_static
    tinyxml_static
)

##############################################################
# define target directory
SET (TARGET_DIRECTORY
    ${CMAKE_CURRENT_LIST_DIR}/../bin
)

##############################################################
# define binary name
SET (TARGET_PROGRAM_NAME
    runecl
)

##############################################################
# include CMake core
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/CMakeCore.txt)