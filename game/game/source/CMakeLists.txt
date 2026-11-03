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
        Protocolo-Caramelo/game/globals/game_core.c
        Protocolo-Caramelo/game/globals/game_cache.c
        Protocolo-Caramelo/game/globals/game_buttons.c
        Protocolo-Caramelo/game/globals/game_state.c

        Protocolo-Caramelo/game/globals/game_menu.c
        Protocolo-Caramelo/game/globals/game_play.c
        Protocolo-Caramelo/game/globals/game_mechanics.c
        Protocolo-Caramelo/game/globals/game_debug.c

        Protocolo-Caramelo/game/globals/game_others.c
        Protocolo-Caramelo/game/game_globals.c
        Protocolo-Caramelo/game/game.c
        #
        Protocolo-Caramelo/inputs/bar.c
        Protocolo-Caramelo/inputs/button.c
        Protocolo-Caramelo/inputs/text.c
        #
        Protocolo-Caramelo/screens/play/monitor.c

        Protocolo-Caramelo/screens/play/monitor/app/taskmgr.c
        Protocolo-Caramelo/screens/play/monitor/app/miner.c
        Protocolo-Caramelo/screens/play/monitor/app/entregas_rapidas.c

        Protocolo-Caramelo/screens/play/monitor/monitor_space.c
        Protocolo-Caramelo/screens/play/monitor/monitor_draw.c
        Protocolo-Caramelo/screens/play/monitor/monitor_text.c
        Protocolo-Caramelo/screens/play/monitor/monitor_cursor.c
        Protocolo-Caramelo/screens/play/monitor/monitor_apps.c
        Protocolo-Caramelo/screens/play/monitor/desktop_icon.c
        Protocolo-Caramelo/screens/play/monitor/icon_layer.c
        Protocolo-Caramelo/screens/play/monitor/desktop_window.c
        Protocolo-Caramelo/screens/play/monitor/click_counter.c
        Protocolo-Caramelo/screens/play/monitor/icon_grid.c
        Protocolo-Caramelo/screens/play/monitor/window_stack.c
        Protocolo-Caramelo/screens/play/monitor/window_manager.c
        Protocolo-Caramelo/screens/play/monitor/window_input.c
        Protocolo-Caramelo/screens/play/monitor/taskbar.c
        Protocolo-Caramelo/screens/play/monitor/monitor_desktop.c
        Protocolo-Caramelo/screens/play/room.c
        Protocolo-Caramelo/screens/menu.c
        Protocolo-Caramelo/screens/play.c
        #
        Protocolo-Caramelo/mechanics/MathProblem/aes128.c
        Protocolo-Caramelo/mechanics/MathProblem/mathproblem.c
        Protocolo-Caramelo/mechanics/MathProblem/mathproblem_generators.c

        Protocolo-Caramelo/mechanics/Clock/clock.c
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

