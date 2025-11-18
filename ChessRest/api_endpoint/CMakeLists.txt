#
#   ENDPOINT
#
cmake_minimum_required(VERSION 3.30)

set(SRC_FILES
    main.cpp
    smakcontroller.cpp
)

setup_dir_vars()


add_executable(PGNdpoint 
    ${SOURCES}
)

target_include_directories(PGNdpoint
    PRIVATE "${INC_DIR}"
    PRIVATE "${oatpp_SOURCE_DIR}/src"
    
)

target_link_libraries(PGNdpoint 
    PUBLIC chessclientlib
)


