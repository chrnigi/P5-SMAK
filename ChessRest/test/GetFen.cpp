#include <dtos/moveDTO.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <cassert>
#include <chesslib.hpp>
#include <models.hpp>
#include <clientlib.hpp>
#include <print>
#include <vector>


#define MOVE_JSON_INPUT "{\"id\":1,\"move_number\":1,\"ply_number\":1,\"piece_moved\":\"P\",\"piece_captured\":null,\"is_promotion\":false,\"is_capture\":false,\"is_enpassant\":false,\"from_square\":12,\"to_square\":28}"

int main(void) {

    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    
    const std::string input(MOVE_JSON_INPUT);

    auto dto = objectMapper->readFromString<oatpp::Object<smak::models::MoveDTO>>(input);

    auto simpleDto = dto->to_simple();

    std::vector<smak::parsing::SimpleMoveDTO> moves = {simpleDto};

    smak::parsing::GameOfFens gof(moves);

    std::string pos = gof.getPositionByPly(1);

    std::println("{}", pos);

    assert(pos == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1");

    return 0;

}