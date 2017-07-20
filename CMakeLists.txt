cmake_minimum_required(VERSION 3.7) #Needs Version 3.8 for boost
project(Green_Engine_2)

set(CMAKE_CXX_STANDARD 11)


#TODO: SET TO FALSE
set(DEVELOPMENT_MODE TRUE CACHE BOOL "Include tests in build.")


file(GLOB_RECURSE SOURCE_FILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/com/ethanbreit/*.c) 

file(GLOB_RECURSE INCLUDE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/com/ethanbreit/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/com/ethanbreit/*.hpp)

set(SOURCE ${SOURCE_FILES} ${INCLUDE_FILES}) #Define Variable "Source" as all Green Engine Library Paths

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/com/ethanbreit) #Define the Green Engine include directories

###
### Sources and Includes
###

if (DEVELOPMENT_MODE)
    message("Development build enabled (not creating library)")

    file(GLOB_RECURSE TEST_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/test/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/test/*.h
            ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/test/*.hpp)

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test)

    add_executable(Green_Engine_2 ${TEST_FILES} ${SOURCE})

    add_definitions(-DGE_DEVELOPMENT_MODE)

else()
    message("Development build disabled (creating library)")


    add_library(Green_Engine_2 ${SOURCE})

endif ()


###
### Libraries
###

set(SUPPORTS_VULKAN FALSE)

if(WIN32)
    set(SUPPORTS_VULKAN TRUE)
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
    set(SUPPORTS_VULKAN TRUE)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/headers) #Define the Green Engine include directories

if (SUPPORTS_VULKAN)
	message("Should support Vulkan")
    find_package(Vulkan REQUIRED)
	if(Vulkan_FOUND)
	    include_directories(${Vulkan_INCLUDE_DIRS})
	    target_link_libraries (Green_Engine_2 ${Vulkan_LIBRARIES})
	else()
		message( FATAL_ERROR "ERROR: Could not Vulkan, Vulkan is NECESSARY for Windows and Linux")
	endif()
elseif(APPLE)
    message("Should support Metal")
   find_library(Metal_LIBRARY Metal)
   target_link_libraries (Green_Engine_2 ${Metal_LIBRARY})

endif()

set_target_properties(Green_Engine_2 PROPERTIES LINKER_LANGUAGE "CXX")
