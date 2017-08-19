function (green_engine_libs includes libs)
###
### Libraries
###

set(SUPPORTS_VULKAN FALSE) # Check if os Supports Vulkan

if(WIN32)# Check if OS is Windows (Supports Vulkan)
    set(SUPPORTS_VULKAN TRUE)
endif()

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux") # Check if OS is Linux (Supports Vulkan)
    set(SUPPORTS_VULKAN TRUE) # Check if os Supports Vulkan
endif()



list(APPEND Green_Engine_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/libs/headers) #Define the Green Engine include directories



find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
    message("${OPENGL_INCLUDE_DIRS} and ${OPENGL_LIBRARIES}\n")
    list(APPEND Green_Engine_INCLUDES ${OPENGL_INCLUDE_DIRS})
    list(APPEND Green_Engine_LIBS ${OPENGL_LIBRARIES})
endif ()


find_package(GLEWosd REQUIRED)

if (GLEW_FOUND)
    message("${GLEW_INCLUDE_DIR} and ${GLEW_LIBRARY}\n")
    list(APPEND Green_Engine_INCLUDES ${GLEW_INCLUDE_DIR})
    list(APPEND Green_Engine_LIBS ${GLEW_LIBRARY})

endif()

find_package(GLFWosd REQUIRED)

if (GLFW_FOUND)
    message("${GLFW_INCLUDE_DIR} and ${GLFW_LIBRARY}\n")
    list(APPEND Green_Engine_INCLUDES ${GLFW_INCLUDE_DIR})
    list(APPEND Green_Engine_LIBS ${GLFW_LIBRARY})

endif()

find_library(Spline TinySpline)

if(Spline)
    message("TinySpline: ${Spline}")
    list(APPEND Green_Engine_LIBS ${Spline})
endif()

if (SUPPORTS_VULKAN)

    message("Should support Vulkan")
    find_package(Vulkan)

    if(Vulkan_FOUND)

        list(APPEND Green_Engine_INCLUDES ${Vulkan_INCLUDE_DIRS})
        list(APPEND Green_Engine_LIBS ${Vulkan_LIBRARIES})

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
    #list(APPEND Green_Engine_INCLUDES ${Vulkan_INCLUDE_DIRS}/Headers)

    list(APPEND Green_Engine_LIBS ${Cocoa_LIBRARY})
    list(APPEND Green_Engine_LIBS ${Core_LIBRARY})
    list(APPEND Green_Engine_LIBS ${Metal_LIBRARY})
    list(APPEND Green_Engine_LIBS ${MetalKit_LIBRARY})
    list(APPEND Green_Engine_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/libs/compiled/mtlpp.o ${CMAKE_CURRENT_SOURCE_DIR}/libs/compiled/window.o )
endif()


    ###
    ### pre-built libs TODO: make build system for them. Also make cross platform
    ###

    #list(APPEND Green_Engine_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/libs/compiled/libz.a )
    find_library(libz z.1)
    find_library(ass assimp)

    message("libz: ${libz}")
    message("assimp: ${ass}")
    list(APPEND Green_Engine_LIBS ${libz})
    list(APPEND Green_Engine_LIBS ${ass})

    set(${includes} ${Green_Engine_INCLUDES} PARENT_SCOPE)
    set(${libs}     ${Green_Engine_LIBS}     PARENT_SCOPE)
    message("libs: ${includes}")
    message("includes: ${libs}")
endfunction()