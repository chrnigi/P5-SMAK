#pragma once

#include <optional>
#include <string>
#ifndef SMAK_CLIENTLIB_HPP
#define SMAK_CLIENTLIB_HPP

#include <oatpp/web/client/ApiClient.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace smak { namespace client {

#include OATPP_CODEGEN_BEGIN(ApiClient)

class SmakClient : public oatpp::web::client::ApiClient {

    API_CLIENT_INIT(SmakClient)
    API_CALL("GET", "games", getAllGames);
    API_CALL("GET", "games/{id}", getGameById, PATH(Int64, id));

    API_CALL("GET", "moves/{id}", getMovesByGameId, PATH(Int64, id));

    std::optional<std::vector<std::string>> getFensFromGame(int64_t gameid);

};

#include OATPP_CODEGEN_END(ApiClient)



}/* client */ }/* smak */

#endif
