#pragma once
#include "chess.hpp"
#include "dtos/evalDTO.hpp"
#include "dtos/moveDTO.hpp"
#include <memory>
#include <optional>
#include <vector>
#ifndef SMAK_SMAKCONTROLLER_HPP
#define SMAK_SMAKCONTROLLER_HPP

#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <clientlib.hpp>
#include <models.hpp>
#include <oatpp/network/tcp/client/ConnectionProvider.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>
#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <ipc.hpp>


namespace smak { namespace controller {

oatpp::Object<models::EvalDTO> evalToDto(Evaluation e);

#include OATPP_CODEGEN_BEGIN(ApiController)

class SmakController : public oatpp::web::server::api::ApiController {

    std::shared_ptr<smak::client::SmakClient> cl;

public:
    SmakController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) : oatpp::web::server::api::ApiController(objectMapper)  {
        auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({"localhost", 8080});
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
        dto1->ply_number=0;
        dto1->piece_moved=smak::models::PiecesEnum::PAWN;
        dto1->from_square=1;
        dto1->to_square=2;
        dto1->piece_captured = smak::models::PiecesEnum::BISHOP;
    
        auto dto2 = smak::models::MoveDTO::createShared();
        dto2->ply_number=0;
        dto2->piece_moved=smak::models::PiecesEnum::BISHOP;
        dto2->from_square=1;
        dto2->to_square=2;
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

public:

    ENDPOINT("GET", "/games/eval/{id}", getGameEval, PATH(Int64, id)) {
        EngineWhisperer ew("stockfish");
        using namespace smak;
        auto incoming_game = cl->getGameById(id);
        auto dto = incoming_game->readBodyToDto<Object<models::GameDTO>>(getDefaultObjectMapper());
        
        auto incoming_moves = cl->getMovesByGameId(id);
        auto move_dtos = incoming_moves->readBodyToDto<oatpp::Vector<Object<models::MoveDTO>>>(getDefaultObjectMapper());
        
        std::vector<chess::Move> chess_moves; 
        chess_moves.reserve(move_dtos->size());
        
        auto int_to_square = [](int sq){
            chess::Rank r = 8-(sq >> 3);
            chess::File f = sq & 7;

            return chess::Square(r, f);
        };
        
        for (auto& m_dto : *move_dtos) {
            
            auto from = int_to_square(m_dto->from_square.getValue(0));
            auto to = int_to_square(m_dto->to_square.getValue(0));

            chess_moves.push_back(chess::Move::make(from, to));
        }

        Evaluation eval;
        if (!ew.start_uci()) {
            return createResponse(Status::CODE_503, "Engine could not start. Analysis unavailable.");
        } 

        std::vector<std::optional<Evaluation>> evals = ew.getEvalsFromGame(chess_moves);

        oatpp::Vector<Object<models::EvalDTO>> eval_dtos{};
        eval_dtos->reserve(chess_moves.size());

        for (auto& e : evals) {
            size_t idx = eval_dtos->size();
            if (e) {
                auto dto = evalToDto(e.value());
                dto->ply = idx;
                dto->id = id;
                eval_dtos->push_back(dto);
            }
            eval_dtos->push_back({});
        }


        return createDtoResponse(Status::CODE_200, eval_dtos);
    }

};

#include OATPP_CODEGEN_END(ApiController)

void run_server();



}/* namespace controller */ }/* namespace smak */

#endif
