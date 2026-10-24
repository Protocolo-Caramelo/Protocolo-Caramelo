add_library(
    graphical
    STATIC
)

target_sources(
    graphical
    PUBLIC
        FILE_SET graphical
        TYPE HEADERS
        FILES
        graphical.h

        GCommons.h
        GInit.h
        GTexture.h
        GWindow.h

        GText/GText.h
        GText/GTextCache.h
        GText/GTextCacheGlyph.h
        GText/GTextChar.h
        GText/GTextCharRect.h
        GText/GTextCharFormat.h
        GText/GTextConfig.h
        GText/GTextSpacing.h
        GText/GTextTypedefs.h

        GTest/GTest.h

    PRIVATE
        GInit.c
        GWindow.c
        GTexture.c

        GText/GText.c
        GText/GTextCache.c
        GText/GTextCacheGlyph.c
        GText/GTextConfig.c
        GText/GTextSpacing.c

        GTest/SimpleWindow.c
        GTest/GTextCG.c
        GTest/GTextCache.c
        GTest/GText_Test1.c
        GTest/GText_Test2.c
        GTest/GText_Test3.c
        GTest/GText_Test4.c
    )
target_link_libraries(
    graphical
    PUBLIC
        utf8
        uthash
        ${graphical_link_libraries}
    )

if(GRAPHICAL_BUILD_TESTS)
    # Habilita GTest/* (veja o #if no topo de cada arquivo de teste)
    target_compile_definitions(
        graphical
        PUBLIC
            COMPILATION_DEVQUOTE
        )
endif()

target_include_directories(
    graphical
    PRIVATE
        ${graphical_include_directories}
    )

target_compile_options(
    graphical
    PUBLIC
        ${graphical_compile_options}
    )
target_compile_features(
    graphical
    PUBLIC
        ${graphical_compile_features}
    )
