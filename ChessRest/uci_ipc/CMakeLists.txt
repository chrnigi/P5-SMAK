cmake_minimum_required(VERSION 3.30)

include(FetchContent)
FetchContent_Declare(
    Boost

    GIT_REPOSITORY 
    https://github.com/boostorg/boost.git

    GIT_TAG
    boost-1.89.0

    GIT_SHALLOW
    true
)

FetchContent_MakeAvailable(Boost)

set(SRC_FILES
    main.cpp
    ipc.cpp
)

setup_dir_vars()

add_executable(uci_ipc ${SOURCES})

target_link_libraries(uci_ipc 
    PUBLIC Boost::system 
    PUBLIC Boost::filesystem 
    PUBLIC fmt
)

message(STATUS "CHESS_HPP: ${CHESS_INCLUDE_DIR}" )


target_include_directories(uci_ipc 
    PRIVATE ${INC_DIR}
    PRIVATE ${CHESS_INCLUDE_DIR}
)
