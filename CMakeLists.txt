cmake_minimum_required (VERSION 3.5.2)
project (libqiniu)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

    if (DEFINED BUILD_SHARED_LIBS AND ${BUILD_SHARED_LIBS})
        add_definitions (-D COMPILING_QINIU_LIBRARY_DLL)
    endif (DEFINED BUILD_SHARED_LIBS AND ${BUILD_SHARED_LIBS})

    if (NOT DEFINED LOCAL_PACKAGE_PATH)
        set (LOCAL_PACKAGE_PATH C:\\packages)
    endif (NOT DEFINED LOCAL_PACKAGE_PATH)

    include_directories (${LOCAL_PACKAGE_PATH}\\include SYSTEM)
    link_directories (${LOCAL_PACKAGE_PATH}\\lib)

else (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

    include_directories (/usr/include /usr/local/include SYSTEM)
    link_directories (/usr/lib /usr/local/lib)

endif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

file (GLOB_RECURSE SOURCE_FILES qiniu/*.c b64/*.c cJSON/*.c windows/*.c)
include_directories (qiniu b64 cJSON windows)
add_library (qiniu ${SOURCE_FILES})
