cmake_minimum_required(VERSION 3.7) #Needs Version 3.8 for boost
project(Green_Engine_2)

set(CMAKE_CXX_STANDARD 11)


#TODO: SET TO FALSE
set(DEVELOPMENT_MODE TRUE CACHE BOOL "Include tests in build.")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake) #Include custom cmake files

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

### STUPID LIB THAT HAS TO BE ADDED TO THE SOURCE


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

set(LIBRARIES)

set(SUPPORTS_VULKAN FALSE) # Check if os Supports Vulkan

if(WIN32)# Check if OS is Windows (Supports Vulkan)
    set(SUPPORTS_VULKAN TRUE)
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux") # Check if OS is Linux (Supports Vulkan)
    set(SUPPORTS_VULKAN TRUE) # Check if os Supports Vulkan
endif()



include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/headers) #Define the Green Engine include directories



find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    message("${OPENGL_INCLUDE_DIRS} and ${OPENGL_LIBRARIES}\n")
    include_directories(${OPENGL_INCLUDE_DIRS})
	set(LIBRARIES "${LIBRARIES}${OPENGL_LIBRARIES};")
    #target_link_libraries (Green_Engine_2 ${OPENGL_LIBRARIES})
endif ()


find_package(GLEWosd REQUIRED)

if (GLEW_FOUND)
    message("${GLEW_INCLUDE_DIR} and ${GLEW_LIBRARY}\n")
    include_directories(${GLEW_INCLUDE_DIR})
	set(LIBRARIES "${LIBRARIES}\"${GLEW_LIBRARY}\";")

    #target_link_libraries (Green_Engine_2 ${GLEW_LIBRARY})

endif()

find_package(GLFWosd REQUIRED)

if (GLFW_FOUND)
    message("${GLFW_INCLUDE_DIR} and ${GLFW_LIBRARY}\n")
    include_directories(${GLFW_INCLUDE_DIR})
	STRING(REGEX REPLACE ";" "" GLFW_LIBRARY ${GLFW_LIBRARY})
	set(LIBRARIES "${LIBRARIES}\"${GLFW_LIBRARY}\";")

    #target_link_libraries (Green_Engine_2 ${GLFW_LIBRARY})

endif()

find_library(Spline TinySpline)

if(Spline)
    message("TinySpline: ${Spline}")

	set(LIBRARIES "${LIBRARIES}\"${Spline}\";")

    #target_link_libraries (Green_Engine_2 ${Spline})
endif()

if (SUPPORTS_VULKAN)

	message("Should support Vulkan")
    find_package(Vulkan)

	if(Vulkan_FOUND)

	    include_directories(${Vulkan_INCLUDE_DIRS})

		set(LIBRARIES "${LIBRARIES}\"${Vulkan_LIBRARIES}\";")

	    #target_link_libraries (Green_Engine_2 ${Vulkan_LIBRARIES})

	else()

		#message( FATAL_ERROR "ERROR: Could not Vulkan, Vulkan is NECESSARY for Windows and Linux")

	endif()

elseif(APPLE)
#TODO: try catch
    message("Should support Metal")
    find_library(Cocoa_LIBRARY Cocoa)
    find_library(Core_LIBRARY CoreFoundation)
    find_library(Metal_LIBRARY Metal)
    find_library(MetalKit_LIBRARY MetalKit)
    message("Cocoa: ${Cocoa_LIBRARY}")
    message("CoreFoundation: ${Core_LIBRARY}")
    message("Metal: ${Metal_LIBRARY}")
    message("MetalKit: ${MetalKit_LIBRARY}")
    #include_directories(${Vulkan_INCLUDE_DIRS}/Headers)

    target_link_libraries (Green_Engine_2 ${Cocoa_LIBRARY})
    target_link_libraries (Green_Engine_2 ${Core_LIBRARY})
    target_link_libraries (Green_Engine_2 ${Metal_LIBRARY})
    target_link_libraries (Green_Engine_2 ${MetalKit_LIBRARY})
    target_link_libraries (Green_Engine_2 ${CMAKE_CURRENT_SOURCE_DIR}/libs/compiled/mtlpp.o ${CMAKE_CURRENT_SOURCE_DIR}/libs/compiled/window.o )
endif()
set_target_properties(Green_Engine_2 PROPERTIES LINKER_LANGUAGE "CXX")

message("Libraries:\n${LIBRARIES}")

target_link_libraries (Green_Engine_2 ${LIBRARIES})

