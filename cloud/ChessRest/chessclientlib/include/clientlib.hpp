#pragma once



#ifndef SMAK_CLIENTLIB_HPP
#define SMAK_CLIENTLIB_HPP

#include <oatpp/web/client/ApiClient.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <models.hpp>
#include <optional>
#include <string>

namespace smak { namespace client {

#include OATPP_CODEGEN_BEGIN(ApiClient)

class SmakClient : public oatpp::web::client::ApiClient {

    API_CLIENT_INIT(SmakClient)

    // Games related stuff
    API_CALL("GET", "games", getAllGames);
    API_CALL("GET", "games/{id}", getGameById, PATH(Int64, id));

    API_CALL("POST", "games", addGame, BODY_DTO(oatpp::Object<models::GameDTO>, game));
    API_CALL("PATCH", "games/{id}", updateGame, PATH(Int64, id), BODY_DTO(oatpp::Object<models::GameDTO>, game));
    
    
    // Moves related stuff
    API_CALL("GET", "moves/{id}", getMovesByGameId, PATH(Int64, id));
    API_CALL("GET", "moves", getAllMoves);
    API_CALL("POST", "moves", addMove, BODY_DTO(oatpp::Object<models::GameDTO>, move));

    std::optional<std::vector<std::string>> getFensFromGame(int64_t gameid);

};

#include OATPP_CODEGEN_END(ApiClient)



}/* client */ }/* smak */

#endif
