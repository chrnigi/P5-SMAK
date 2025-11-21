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
)

setup_dir_vars()

add_executable(uci_ipc ${SOURCES})

target_link_libraries(uci_ipc PUBLIC Boost::system Boost::filesystem fmt)
