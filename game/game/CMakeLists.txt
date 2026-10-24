cmake_minimum_required(VERSION 4.2.3)
project(Protocolo-Caramelo C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
if(NOT ${compilation_mode} MATCHES "TOOLCHAIN")
    set(CMAKE_C_COMPILER /usr/bin/gcc)
elseif(${compilation_mode} MATCHES "TOOLCHAIN_WINDOWS")
    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_C_COMPILER /usr/bin/x86_64-w64-mingw32-gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/x86_64-w64-mingw32-g++)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(main_include_directories app/header)
set(main_link_directories)
set(main_link_libraries m graphical SDL2 SDL2_image)
set(main_link_options)
#
set(graphical_include_directories)
set(graphical_link_directories)
set(graphical_link_libraries SDL2 SDL2_image SDL2_ttf utf8 uthash m)
set(graphical_link_options)
#
set(
    libsdl2-mingw
    /opt/sdl-mingw/SDL2-2.32.10/x86_64-w64-mingw32/lib
    /opt/sdl-mingw/SDL2_image-2.8.8/x86_64-w64-mingw32/lib
    /opt/sdl-mingw/SDL2_ttf-2.24.0/x86_64-w64-mingw32/lib
)
set(
    libsdl2_include-mingw
    /opt/sdl-mingw/SDL2-2.32.10/x86_64-w64-mingw32/include
    /opt/sdl-mingw/SDL2-2.32.10/x86_64-w64-mingw32/include/SDL2
    /opt/sdl-mingw/SDL2_image-2.8.8/x86_64-w64-mingw32/include
    /opt/sdl-mingw/SDL2_ttf-2.24.0/x86_64-w64-mingw32/include
)
set(
    mingw_link_libraries
    -lmingw32
    -lSDL2main
    -lSDL2
    -static
    #
    -lsetupapi
    -lwinmm
    -limm32
    -lole32
    -loleaut32
    -luuid
    -lversion
    -lrpcrt4
    -mwindows
)
#
set(compile_options -Wall -Wextra -D COMPILATION_NORMAL)
set(compile_features c_std_99)

# Check compilation mode
if(NOT DEFINED compilation_mode)
    set(compilation_mode "NORMAL")
endif()

# 
if(${compilation_mode} STREQUAL "NORMAL")
elseif(${compilation_mode} STREQUAL "DEVQUOTE")
    set(compile_options
        -Wall
        -Wextra
        # -fsanitize=address,undefined -fno-omit-frame-pointer
        -g -O0
        # -O4
        -D COMPILATION_DEVQUOTE
    )
    set(compile_features c_std_99)
    set(
        main_link_options 
        # -fsanitize=address,undefined -fno-omit-frame-pointer
    )
elseif(${compilation_mode} STREQUAL TOOLCHAIN_WINDOWS_DEVQUOTE OR ${compilation_mode} STREQUAL TOOLCHAIN_WINDOWS)
    if(NOT ${compilation_mode} STREQUAL "TOOLCHAIN_WINDOWS_DEVQUOTE")
        set(compile_options -Wall -Wextra -D COMPILATION_TOOLCHAIN_WINDOWS)
    else()
        set(compile_options -Wall -Wextra -D COMPILATION_TOOLCHAIN_WINDOWS_DEVQUOTE)
    endif()
    set(compile_features c_std_99)

    set(graphical_include_directories ${graphical_include_directories} ${libsdl2_include-mingw})
    set(graphical_link_directories ${graphical_link_directories} ${libsdl2-mingw})
    set(graphical_link_libraries ${graphical_link_libraries} ${mingw_link_libraries})
    #
    set(main_include_directories ${main_include_directories} ${libsdl2_include-mingw})
    set(main_link_directories ${main_link_directories} ${libsdl2-mingw})
    set(main_link_libraries ${main_link_libraries} ${mingw_link_libraries})
endif()

# Add subdirectories
add_subdirectory(libgraphical)
add_subdirectory(source)
