if(${compilation_mode} STREQUAL "NORMAL")
    set(MAIN_NAME __${CMAKE_PROJECT_NAME}__)
elseif(${compilation_mode} STREQUAL "DEVQUOTE")
    set(MAIN_NAME __${CMAKE_PROJECT_NAME}_devquote__)
elseif(${compilation_mode} STREQUAL "TOOLCHAIN_WINDOWS")
    set(MAIN_NAME __${CMAKE_PROJECT_NAME}_windows__)
elseif(${compilation_mode} STREQUAL "TOOLCHAIN_WINDOWS_DEVQUOTE")
    set(MAIN_NAME __${CMAKE_PROJECT_NAME}_windows_devquote__)
endif()

add_executable(${MAIN_NAME})

target_sources(
    ${MAIN_NAME}
    PRIVATE
        main.c
        #
        Protocolo-Caramelo/elements/elements_globals.c
        #
        Protocolo-Caramelo/inputs/bar.c
        Protocolo-Caramelo/inputs/button.c
        Protocolo-Caramelo/inputs/text.c
        #
        Protocolo-Caramelo/screens/menu.c
    )
target_include_directories(
    ${MAIN_NAME}
    PRIVATE
        ${main_include_directories}
        Protocolo-Caramelo
    )
target_link_directories(
    ${MAIN_NAME}
    PRIVATE
        ${main_link_directories}
    )
target_link_libraries(
    ${MAIN_NAME}
    PRIVATE
        ${main_link_libraries}
    )

target_compile_options(
    ${MAIN_NAME}
    PRIVATE
        ${compile_options}
    )
target_link_options(
    ${MAIN_NAME}
    PRIVATE
        ${main_link_options}
    )
target_compile_features(
    ${MAIN_NAME}
    PRIVATE
        ${compile_features}
    )

