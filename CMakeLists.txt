cmake_minimum_required (VERSION 3.5.2)
project (libqiniu)

set (BUILD_SHARED_LIBS ON CACHE BOOL "Set to ON to build shared or dll libraries (default: YES)")
set (FUNCTION_LEVEL_LINKING OFF CACHE BOOL "Set to ON to place every function in its own section to garbadge-collect unreferenced ones")

file (GLOB_RECURSE MY_SOURCE_FILES qiniu/*.c b64/*.c cJSON/*.c)
include_directories (qiniu b64 cJSON)

### Platform Settings

set (MY_COMPILE_DEFINITIONS "")

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")

    ## Append platform-dependent source files
    file (GLOB_RECURSE MY_WINDOWS_SOURCE_FILES windows/*.c)
    list (APPEND MY_SOURCE_FILES ${MY_WINDOWS_SOURCE_FILES})
    include_directories (windows)

    set (LOCAL_PACKAGE_PATH "C:/packages" CACHE PATH "Set to the path where dependent libraries (openssl / curl, etc) reside")
    include_directories (${LOCAL_PACKAGE_PATH}/include SYSTEM)
    link_directories (${LOCAL_PACKAGE_PATH}/lib)

    if (DEFINED BUILD_SHARED_LIBS AND ${BUILD_SHARED_LIBS})
        list (APPEND MY_COMPILE_DEFINITIONS COMPILING_QINIU_LIBRARY_DLL)
    endif (DEFINED BUILD_SHARED_LIBS AND ${BUILD_SHARED_LIBS})

else ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")

    include_directories (/usr/include /usr/local/include SYSTEM)
    link_directories (/usr/lib /usr/local/lib)

    set (MY_LINKING_LIBRARIES curl crypto)

    if (DEFINED FUNCTION_LEVEL_LINKING AND ${FUNCTION_LEVEL_LINKING})
        set_target_properties (qiniu PROPERTIES COMPILE_FLAGS -ffunction-sections)
    endif (DEFINED FUNCTION_LEVEL_LINKING AND ${FUNCTION_LEVEL_LINKING})

endif ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")

### Target Settings

if (DEFINED BUILD_SHARED_LIBS AND ${BUILD_SHARED_LIBS})

    add_library (qiniu SHARED ${MY_SOURCE_FILES})
    
    if (NOT ("X${MY_LINKING_LIBRARIES}" STREQUAL "X"))
        target_link_libraries (qiniu ${MY_LINKING_LIBRARIES})
    endif (NOT ("X${MY_LINKING_LIBRARIES}" STREQUAL "X"))

else (DEFINED BUILD_SHARED_LIBS AND ${BUILD_SHARED_LIBS})

    add_library (qiniu STATIC ${MY_SOURCE_FILES})

endif (DEFINED BUILD_SHARED_LIBS AND ${BUILD_SHARED_LIBS})

if (NOT ("X${MY_COMPILE_DEFINITIONS}" STREQUAL "X"))
    set_target_properties (qiniu PROPERTIES COMPILE_DEFINITIONS ${MY_COMPILE_DEFINITIONS})
endif (NOT ("X${MY_COMPILE_DEFINITIONS}" STREQUAL "X"))
