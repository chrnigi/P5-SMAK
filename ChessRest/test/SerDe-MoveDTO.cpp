#include <cassert>
#include <models.hpp>
#include <clientlib.hpp>
#include <chesslib.hpp>
#include <fmt/core.h>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

// E2E4
#define MOVE_JSON_INPUT "{\"id\":1,\"move_number\":1,\"ply_number\":1,\"piece_moved\":\"P\",\"piece_captured\":null,\"is_promotion\":false,\"is_capture\":false,\"is_enpassant\":false,\"is_castles\":false,\"from_square\":12,\"to_square\":28}"

int main(void) {
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();

    std::string input(MOVE_JSON_INPUT);

    auto dto = smak::models::MoveDTO::createShared();

    dto->id = 1;
    dto->move_number = 1;
    dto->ply_number = 1;
    dto->piece_moved = smak::models::PiecesEnum::PAWN;
    dto->piece_captured = nullptr;
    dto->is_promotion = false;
    dto->is_capture = false;
    dto->is_enpassant = false;
    dto->is_castles = false;
    dto->from_square = 12; // E2
    dto->to_square = 28; // E4


    auto dto_from_str = objectMapper->readFromString<oatpp::Object<smak::models::MoveDTO>>(input);

    assert(dto->id == dto_from_str->id);
    assert(dto->move_number == dto_from_str->move_number);
    assert(dto->ply_number == dto_from_str->ply_number);
    assert(dto->piece_moved == dto_from_str->piece_moved);
    assert(dto->is_promotion == dto_from_str->is_promotion);
    assert(dto->is_capture == dto_from_str->is_capture);
    assert(dto->is_enpassant == dto_from_str->is_enpassant);
    assert(dto->from_square == dto_from_str->from_square);
    assert(dto->to_square == dto_from_str->to_square);
    
    auto serialized = objectMapper->writeToString(dto);
    fmt::println("{}", std::string(serialized->c_str()));
    assert(serialized == MOVE_JSON_INPUT);

    return 0;

}