cmake_minimum_required(VERSION 3.8) #we don't use BOOST anymore...
project(Green_Engine_2)



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

    #add_definitions(-DGE_DEVELOPMENT_MODE)

else()
    message("Development build disabled (creating library)")
	add_definitions(-DGE_DEVELOPMENT_MODE)


    add_library(Green_Engine_2 ${SOURCE})

endif ()


if (WIN32)
find_library(ws2_32_LIBRARY_PATH ws2_32)
target_link_libraries(Green_Engine_2 ${ws2_32_LIBRARY_PATH})
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Libraries.cmake)
green_engine_libs(includes libraries)

include_directories(${includes})
target_link_libraries(Green_Engine_2 ${libraries})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/res/)


get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

###
### Include res into build path
###

file(COPY "res" DESTINATION "${CMAKE_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)


set_target_properties(Green_Engine_2 PROPERTIES LINKER_LANGUAGE "CXX")
