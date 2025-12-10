#include "oatpp/core/Types.hpp"
#include <dtos/moveDTO.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <cassert>
#include <chesslib.hpp>
#include <models.hpp>
#include <clientlib.hpp>
#include <fmt/core.h>


#define MOVE_JSON_INPUT "{\"id\":1,\"ply_number\":1,\"move_type\":\"normal\",\"piece_moved\":\"P\",\"piece_captured\":null,\"from_square\":12,\"to_square\":28}"

int main(void) {

    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    
    const std::string input(MOVE_JSON_INPUT);

    auto dto = objectMapper->readFromString<oatpp::Object<smak::models::MoveDTO>>(input);

    oatpp::Vector<oatpp::Object<smak::models::MoveDTO>> moves = {dto};

    smak::parsing::GameOfFens gof(moves);

    std::string pos = gof.getPositionByPly(1);

    fmt::println("{}", pos);

    assert(pos == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

    return 0;

}
