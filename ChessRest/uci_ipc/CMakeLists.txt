set(CMAKE_POLICY_VERSION_MINIMUM 3.30.0)
cmake_minimum_required(VERSION 3.30)

# include(FetchContent)
# FetchContent_Declare(
#     Boost
#     URL https://github.com/boostorg/boost/releases/download/boost-1.89.0/boost-1.89.0-cmake.tar.gz
#     DOWNLOAD_EXTRACT_TIMESTAMP ON
    
# )

#set(BOOST_ENABLE_CMAKE ON)

set(SRC_FILES
    ipc.cpp
    uci_commands.cpp
    evaluation.cpp
)

setup_dir_vars()

add_library(uci_ipc ${SOURCES})

target_link_libraries(uci_ipc 
    PUBLIC Boost::system 
    PUBLIC Boost::regex
    PUBLIC Boost::process
    PUBLIC fmt
    plog::plog
)

message(STATUS "CHESS_HPP: ${CHESS_INCLUDE_DIR}" )


target_include_directories(uci_ipc 
    PUBLIC ${INC_DIR}
    PUBLIC ${CHESS_INCLUDE_DIR}
)

make_directory(${CMAKE_CURRENT_SOURCE_DIR}/docs)

doxygen_add_docs(
    uci_ipc_docs
    #ALL
    CONFIG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
)
