cmake_minimum_required(VERSION 3.30)

enable_testing()

# Test serialization and deserialization for GameDTO
add_executable(SerDe_test_game
    SerDe-GameDTO.cpp
)
target_link_libraries(SerDe_test_game
    PUBLIC chessclientlib
    PRIVATE oatpp
    PUBLIC fmt
)
add_test(
    NAME SerDe_test_game
    COMMAND SerDe_test_game
)

# Test serialization and deserialization for MoveDTO
add_executable(SerDe_test_move
    SerDe-MoveDTO.cpp
)
target_link_libraries(SerDe_test_move
    PUBLIC chessclientlib
    PRIVATE oatpp
    PUBLIC fmt
)

add_test(
    NAME SerDe_test_move
    COMMAND SerDe_test_move
)


# Test deserializing a move - 1.E2E4, converting it to a SimpleMoveDTO and parsing the move to get the position after the move.
add_executable(GetFenByPly
    GetFen.cpp
)
target_link_libraries(GetFenByPly
    PUBLIC chessclientlib
    PRIVATE oatpp
    PUBLIC fmt
)
add_test(
    NAME GetFenByPly
    COMMAND GetFenByPly
)
