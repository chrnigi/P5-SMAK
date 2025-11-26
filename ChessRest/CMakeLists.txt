set(CMAKE_POLICY_VERSION_MINIMUM 3.30.0)
cmake_minimum_required(VERSION 3.30)

project(pgnservice LANGUAGES CXX)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TEST_DIR "${BASE_DIR}/test")
set(DOCS_DIR "${BASE_DIR}/docs")

find_package(Doxygen)

include(FetchContent)

FetchContent_Declare(
    oatpp
    GIT_REPOSITORY https://github.com/oatpp/oatpp.git
    GIT_TAG 17ef2a7f6c8a932498799b2a5ae5aab2869975c7
    FIND_PACKAGE_ARGS NAMES oatpp
)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e424e3f2e607da02742f73db84873b8084fc714c # 12.0.0
) 
FetchContent_MakeAvailable(fmt)

add_compile_options("-O") # oatpp complains without -O
set(OATPP_BUILD_TESTS false)
FetchContent_MakeAvailable(oatpp)

include(FetchContent)

FetchContent_Declare(
    plog
    GIT_REPOSITORY https://github.com/SergiusTheBest/plog
    GIT_TAG        1.1.10
    GIT_SHALLOW    true
)

FetchContent_MakeAvailable(plog)

include(FetchContent)
FetchContent_Declare(
    Boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.89.0/boost-1.89.0-cmake.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    OVERRIDE_FIND_PACKAGE
    
)
set(BOOST_PROCESS_USE_STD_FS true)
add_compile_definitions(BOOST_PROCESS_USE_STD_FS ) 
set(BOOST_INCLUDE_LIBRARIES
    asio system process regex
)
FetchContent_MakeAvailable(Boost)

# find_package(
#     Boost
#     1.89.0    
#     EXACT
#     REQUIRED
#     COMPONENTS asio filesystem system process regex
# )


macro(setup_dir_vars)
    set(SRC_DIR 
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )
    set(INC_DIR
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )

    foreach(F IN LISTS SRC_FILES)
        list(APPEND SOURCES "${SRC_DIR}/${F}")
    endforeach()
endmacro()

set(CHESS_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

file(DOWNLOAD 
    https://github.com/Disservin/chess-library/raw/refs/heads/master/include/chess.hpp "${CHESS_INCLUDE_DIR}/chess.hpp"
)


add_subdirectory(chessclientlib)
add_subdirectory(api_endpoint)
add_subdirectory("test")
add_subdirectory(uci_ipc)
