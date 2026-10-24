add_library(
    uthash INTERFACE
)

target_include_directories(
    uthash
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)
