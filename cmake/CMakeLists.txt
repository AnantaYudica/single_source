cmake_minimum_required (VERSION 3.0.2)

add_compile_options(-std=c++11)

project(single_source)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/basic CACHE PATH "..." FORCE)
endif()

include_directories (${PROJECT_SOURCE_DIR}/../include)

function(setSourceGroup strSourceGroup strDir)
    file(GLOB listFiles "${strDir}/*")
    foreach(it ${listFiles})
        get_filename_component(nameFile ${it} NAME)
        get_filename_component(extFile ${it} EXT)
        if(IS_DIRECTORY ${it})  
            setSourceGroup("${strSourceGroup}\\${nameFile}"
                "${strDir}/${nameFile}")
        elseif("${extFile}" MATCHES "\\.h")
            source_group(${strSourceGroup} FILES ${it})
        endif()
    endforeach()
endfunction(setSourceGroup)

file(GLOB_RECURSE LIST_INCLUDE "${PROJECT_SOURCE_DIR}/../include/*")
file(GLOB_RECURSE LIST_SRC "${PROJECT_SOURCE_DIR}/../src/*")

setSourceGroup("Header Files" "${PROJECT_SOURCE_DIR}/../include")
setSourceGroup("Source Files" "${PROJECT_SOURCE_DIR}/../src")

add_executable("single_source" ${LIST_INCLUDE} ${LIST_SRC})
