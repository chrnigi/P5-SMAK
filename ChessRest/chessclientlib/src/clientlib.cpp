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
    

    parsing::GameOfFens gof(dto_vec);

    return std::optional<std::vector<std::string>>(gof.getAllPositions());


}

}
