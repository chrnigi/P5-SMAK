#pragma once

#ifndef SMAK_SMAKCONTROLLER_HPP
#define SMAK_SMAKCONTROLLER_HPP

#include <memory>
#include <optional>
#include <vector>
#include <sstream>

#include <chess.hpp>
#include <clientlib.hpp>
#include <chesslib.hpp>
#include <ipc.hpp>
#include <models.hpp>

#include <oatpp/core/Types.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/tcp/client/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

namespace smak {
namespace controller {

oatpp::Object<models::EvalDTO> evalToDto(Evaluation e);

#include OATPP_CODEGEN_BEGIN(ApiController)

class SmakController : public oatpp::web::server::api::ApiController {

    std::shared_ptr<smak::client::SmakClient> cl;

public:
    SmakController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper) {        auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({"localhost", 8080});
        auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(connectionProvider);
        auto clientObjectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        
        cl = smak::client::SmakClient::createShared(requestExecutor,clientObjectMapper);
    }

public:
    ENDPOINT("GET", "/games", root) { return createResponse(Status::CODE_501); }

public:
    ENDPOINT("GET", "/moves/{id}", movesByGameId, PATH(Int64, id)) {
        OATPP_LOGD("/moves/{id}", " id=%li", id.getValue(0))
        return createResponse(Status::CODE_501);
    }

public:
    ENDPOINT("GET", "/moves/{id}/ply", moveByIdAndPly, PATH(Int64, id), QUERY(Int32, ply_number, "ply")) {
        return createResponse(Status::CODE_501);
    }

public:
    ADD_CORS(getGameEval, "*", "GET",
        "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range",
        "1728000");
    ENDPOINT("GET", "/games/eval/{id}", getGameEval, PATH(Int64, id)) {
        using namespace smak;
        using namespace chess;

        auto incoming_game = cl->getGameById(id);

        if (incoming_game->getStatusCode() != Status::CODE_200.code) {
            return createResponse(Status::CODE_503,"Unable to retrieve game from database.");
        }
        auto dto = incoming_game->readBodyToDto<Object<models::GameDTO>>(getDefaultObjectMapper());
        auto incoming_moves = cl->getMovesByGameId(id);
        auto move_dtos =
            incoming_moves->readBodyToDto<oatpp::Vector<Object<models::MoveDTO>>>(
                getDefaultObjectMapper());

        if (incoming_moves->getStatusCode() != Status::CODE_200.code) {
            return createResponse(Status::CODE_503, "Unable to retrieve moves from database.");
        }

        std::vector<chess::Move> chess_moves{};
        chess_moves.reserve(move_dtos->size());

        auto int_to_square = [](int sq) {
            chess::Rank r = 7 - (sq >> 3);
            chess::File f = sq & 7;

            return chess::Square(r, f);
        };

        for (auto &m_dto : *move_dtos) {

            auto from = int_to_square(m_dto->from_square.getValue(0));
            auto to = int_to_square(m_dto->to_square.getValue(0));

            chess_moves.push_back(chess::Move::make(from, to));
        }
        EngineWhisperer ew("stockfish");

        Evaluation eval;
        if (!ew.start_uci()) {
        return createResponse(Status::CODE_503, "Engine could not start. Analysis unavailable.");
        }

        std::vector<std::optional<Evaluation>> evals =
            ew.getEvalsFromGame(chess_moves);

        auto eval_dtos = oatpp::Vector<Object<models::EvalDTO>>::createShared();
        eval_dtos->reserve(chess_moves.size());

        for (auto &e : evals) {
            size_t idx = eval_dtos->size();
            if (e) {
                auto dto = evalToDto(e.value());
                dto->ply = idx + 1;
                dto->id = id;
                eval_dtos->push_back(dto);
                if (chess_moves.size() > idx) {
                    eval_dtos->back()->move = chess::uci::moveToUci(chess_moves[idx]);
                }
            } else {
                eval_dtos->push_back({});
            }
        }

            return createDtoResponse(Status::CODE_200, eval_dtos);
    }
public:
    ENDPOINT_INFO(pgnEval) {
        info->addConsumes<String>("application/x-chess-pgn");
    }
    ADD_CORS(pgnEval, "*", "GET",
        "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range",
        "1728000");
    ENDPOINT("POST", "/games/pgn", pgnEval, BODY_STRING(String, pgn), HEADER(String, contentType, "Content-Type")) {
        using namespace smak;
        using namespace chess;
        if (contentType->find("application/x-chess-pgn") == -1) {
            return createResponse(Status::CODE_415, "Unsupported Content-Type.");
        }
        std::string s("[]\n");
        s.append( + pgn->c_str());
        
        std::stringstream pgn_stream(s);

        smak::parsing::PgnParser parser(pgn_stream);
        parser.parse();

        auto parsed = parser.getMoves();

        if (parsed.value().size()) {
            EngineWhisperer ew("stockfish");
            if (!ew.start_uci()) {
                return createResponse(Status::CODE_503,
                "Engine could not start. Analysis unavailable.");
            }
            OATPP_LOGD(__PRETTY_FUNCTION__, "attempting engine eval");
            auto evals = ew.getEvalsFromGame(parsed.value());
            auto eval_dtos = oatpp::Vector<Object<models::EvalDTO>>::createShared();
            eval_dtos->reserve(parsed.value().size());

            for (auto &e : evals) {
                size_t idx = eval_dtos->size();
                if (e) {
                    auto dto = evalToDto(e.value());
                    dto->ply = idx + 1;
                    dto->id = (v_int64){0};
                    eval_dtos->push_back(dto);
                    if (parsed.value().size() > idx) {
                    eval_dtos->back()->move = chess::uci::moveToUci(parsed.value()[idx]);
                    }
                } else {
                eval_dtos->push_back({});
                }
            }
            return createDtoResponse(Status::CODE_200, eval_dtos);
        }
        return createResponse(Status::CODE_400, "PGN was invalid.");
    }
  
};

#include OATPP_CODEGEN_END(ApiController)

void run_server();

} /* namespace controller */
} /* namespace smak */

#endif
