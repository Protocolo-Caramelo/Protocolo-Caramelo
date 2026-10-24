add_library(
    utf8 INTERFACE
)
target_include_directories(
    utf8
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:utf8>
)
