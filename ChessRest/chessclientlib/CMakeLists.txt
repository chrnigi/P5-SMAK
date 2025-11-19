cmake_minimum_required(VERSION 3.30.0)

set(SRC_FILES
    clientlib.cpp
    chesslib.cpp
)

setup_dir_vars()

file(DOWNLOAD 
    https://github.com/Disservin/chess-library/raw/refs/heads/master/include/chess.hpp "${INC_DIR}/chess.hpp"
)

add_library(chessclientlib ${SOURCES})

target_link_libraries(chessclientlib
    PUBLIC oatpp
)

target_include_directories(chessclientlib
    PRIVATE "${oatpp_SOURCE_DIR}/src"
    PUBLIC  "${INC_DIR}"
)
