cmake_minimum_required(VERSION 3.23) # target_sources(... FILE_SET ...) exige CMake 3.23+

if(NOT DEFINED CMAKE_PROJECT_NAME)
    project(graphical LANGUAGES C)
endif()

# O codigo usa extensoes do GNU C (inicializador vazio "{}", "##__VA_ARGS__"),
# por isso C11 com extensoes. Se a biblioteca for incluida por outro projeto
# (add_subdirectory), o padrao de C fica a cargo dele.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS ON)
endif()

option(GRAPHICAL_BUILD_TESTS "Compila as rotinas de teste interativas (GTest/*)" OFF)

# Bibliotecas das quais "graphical" depende. Um projeto pai pode definir
# graphical_link_libraries antes de add_subdirectory() para usar as suas.
if(NOT DEFINED graphical_link_libraries)
    find_package(SDL2 QUIET CONFIG)
    find_package(SDL2_image QUIET CONFIG)
    find_package(SDL2_ttf QUIET CONFIG)

    if(SDL2_FOUND AND SDL2_image_FOUND AND SDL2_ttf_FOUND)
        set(graphical_link_libraries SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)
    else()
        # Sistemas sem arquivos de configuracao do CMake: usa o pkg-config
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(GRAPHICAL_SDL REQUIRED IMPORTED_TARGET sdl2 SDL2_image SDL2_ttf)
        set(graphical_link_libraries PkgConfig::GRAPHICAL_SDL)
    endif()

    if(UNIX AND NOT APPLE)
        list(APPEND graphical_link_libraries m)
    endif()
endif()

if(NOT DEFINED graphical_include_directories)
    set(graphical_include_directories GText GTest)
endif()

add_subdirectory(utf8)
add_subdirectory(uthash)
add_subdirectory(graphical)
