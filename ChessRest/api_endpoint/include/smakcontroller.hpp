#pragma once
#ifndef SMAK_SMAKCONTROLLER_HPP
#define SMAK_SMAKCONTROLLER_HPP

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <clientlib.hpp>
#include <models.hpp>
#include <oatpp/network/tcp/client/ConnectionProvider.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>
#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>


namespace smak { namespace controller {

#include OATPP_CODEGEN_BEGIN(ApiController)

class SmakController : public oatpp::web::server::api::ApiController {

    std::shared_ptr<smak::client::SmakClient> cl;

public:
    SmakController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) : oatpp::web::server::api::ApiController(objectMapper)  {
        auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({"130.225.39.244", 8080});
        auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(connectionProvider);
        auto clientObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        cl = smak::client::SmakClient::createShared(requestExecutor, clientObjectMapper);
    }

public:
    ENDPOINT("GET", "/games", root) {
        auto dto = smak::models::GameDTO::createShared();
        dto->gamestart = "1970-01-01";
        dto->gamestate = smak::models::GameStateEnum::NOT_STARTED;
        dto->id = 1;

        auto data = cl->getGameById(1);

        auto data_str = data->readBodyToString();

        OATPP_LOGD("/games", " Received body from DB: %s", data_str->c_str())

        OATPP_LOGD("/games", " Sent game DTO as response!");

        return createDtoResponse(Status::CODE_200, dto);
    }

public:
    ENDPOINT("GET", "/moves/{id}", movesByGameId, PATH(Int64, id)) {
        
        OATPP_LOGD("/moves/{id}", " id=%li", id.getValue(0) )

        auto dto = oatpp::Vector<oatpp::Object<smak::models::MoveDTO>>::createShared();

        auto dto1 = smak::models::MoveDTO::createShared();
        dto1->move_number=1;
        dto1->ply_number=0;
        dto1->piece_moved=smak::models::PiecesEnum::PAWN;
        dto1->from_square=1;
        dto1->to_square=2;
        dto1->is_promotion = false;
        dto1->piece_captured = smak::models::PiecesEnum::BISHOP;
    
        auto dto2 = smak::models::MoveDTO::createShared();
        dto2->move_number=1;
        dto2->ply_number=0;
        dto2->piece_moved=smak::models::PiecesEnum::BISHOP;
        dto2->from_square=1;
        dto2->to_square=2;
        dto2->is_promotion = false;
        dto2->piece_captured = smak::models::PiecesEnum::PAWN;

        dto->push_back(dto1);
        dto->push_back(dto2);

        return createDtoResponse(Status::CODE_200, dto);
    }

public:

    ENDPOINT("GET", "/moves/{id}/ply", moveByIdAndPly, PATH(Int64, id), QUERY(Int32, ply_number, "ply")) {
        
        OATPP_LOGD("/moves/{id}?ply=", " Incoming query for Ply %i, in game %li", ply_number.getValue(0), id.getValue(0));

        return createResponse(Status::CODE_200, "nej");
    }

};

#include OATPP_CODEGEN_END(ApiController)

void run_server();

}/* namespace controller */ }/* namespace smak */

#endif
