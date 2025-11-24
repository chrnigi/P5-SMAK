cmake_minimum_required(VERSION 3.30)

include(FetchContent)
FetchContent_Declare(
    Boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.89.0/boost-1.89.0-cmake.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    
)


set(BOOST_INCLUDE_LIBRARIES
    asio filesystem system process
)
FetchContent_MakeAvailable(Boost)

find_package(Doxygen)


set(SRC_FILES
    main.cpp
    ipc.cpp
    uci_commands.cpp
    evaluation.cpp
)

setup_dir_vars()

add_executable(uci_ipc ${SOURCES})

target_link_libraries(uci_ipc 
    PUBLIC Boost::system 
    PUBLIC Boost::filesystem 
    PUBLIC fmt
    plog::plog
)

message(STATUS "CHESS_HPP: ${CHESS_INCLUDE_DIR}" )


target_include_directories(uci_ipc 
    PRIVATE ${INC_DIR}
    PRIVATE ${CHESS_INCLUDE_DIR}
)

doxygen_add_docs(
    uci_ipc_docs
    CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
)
