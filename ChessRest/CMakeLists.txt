set(CMAKE_POLICY_VERSION_MINIMUM 3.30.0)
cmake_minimum_required(VERSION 3.30)

project(pgnservice LANGUAGES CXX)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TEST_DIR "${BASE_DIR}/test")

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
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281 # 10.2.1
) 
FetchContent_MakeAvailable(fmt)

add_compile_options("-O") # oatpp complains without -O
set(OATPP_BUILD_TESTS false)
FetchContent_MakeAvailable(oatpp)

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


add_subdirectory(chessclientlib)
add_subdirectory(api_endpoint)
add_subdirectory("test")
add_subdirectory(uci_ipc)
