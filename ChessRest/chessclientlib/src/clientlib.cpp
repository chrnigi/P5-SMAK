#include <chesslib.hpp>
#include "dtos/moveDTO.hpp"
#include "oatpp/core/Types.hpp"
#include <clientlib.hpp>
#include <models.hpp>
#include <optional>
#include <string>
#include <vector>

namespace smak::client {

std::optional<std::vector<std::string>> SmakClient::getFensFromGame(int64_t gameid) {
    auto resp = getMovesByGameId(gameid);
    auto dto_vec = resp->readBodyToDto<oatpp::Vector<oatpp::Object<smak::models::MoveDTO>>>(getObjectMapper());
    std::vector<parsing::SimpleMoveDTO> s_vec;
    s_vec.reserve(dto_vec->size());
    
    for (auto& m : *dto_vec) {
        s_vec.push_back(m->to_simple());
    }
    parsing::GameOfFens gof(s_vec);

    return std::optional<std::vector<std::string>>(gof.getAllPositions());


}

}
