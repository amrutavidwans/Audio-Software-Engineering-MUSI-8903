cmake_minimum_required (VERSION 3.0)
project (MUSI8903
        VERSION 0.0.0.1)

#######################################################################################################
## includes
# where to look for the default stuff...
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake.modules")

# platform detection
include (SetPlatform)

# output directories
include (SetOutputDir)

# oslibs
include (SetOsLibs)

# helper functions
include(AddSrcDirectories)
include(AddIncDirectories)


#######################################################################################################
## project specific stuff

# set directories
set (PROJECT_DIRECTORIES)
list (APPEND PROJECT_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/${PROJECT_NAME}Exec)       
list (APPEND PROJECT_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/AudioFileIO) 
list (APPEND PROJECT_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/CombFilter) 
list (APPEND PROJECT_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/Tests/TestExec)
list (APPEND PROJECT_DIRECTORIES ${CMAKE_SOURCE_DIR}/src/Vibrato)
    
# set includes
set (PROJECT_INCLUDES)
list (APPEND PROJECT_INCLUDES ${CMAKE_SOURCE_DIR}/inc)       
list (APPEND PROJECT_INCLUDES ${CMAKE_SOURCE_DIR}/src/inc)       
list (APPEND PROJECT_INCLUDES ${CMAKE_BINARY_DIR}/inc)       

# add directories
add_src_directories ()


# output version number variable
message(STATUS ${PROJECT_NAME} Version: ${PROJECT_VERSION})
message(STATUS "")

# generate platform config file
configure_file(${CMAKE_SOURCE_DIR}/src/${PROJECT_NAME}Config.h.in ${CMAKE_BINARY_DIR}/inc/${PROJECT_NAME}Config.h)
# integrate cppcheck targets in IDE
option (WITH_CPPCHECKTARGET "Add target for cppcheck testing" ON)
#######################################################################################################
# ## tests

# with tests or not
option (WITH_TESTS "Use UnitTest++ for testing" ON)

if (WITH_TESTS)
    include(AddCppcheckTest)
    enable_testing()
    
    # add tests here
    add_test(AudioIo ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestExec AudioIo "${CMAKE_SOURCE_DIR}/src/Tests/TestData/")
    add_test(CombFilter ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestExec CombFilter)
    add_test(RingBuff ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestExec RingBuff)
    add_cppcheck_test ()
endif (WITH_TESTS)

