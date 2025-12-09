#pragma once

#ifndef SMAK_EVALDTO_HPP
#define SMAK_EVALDTO_HPP

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <chess.hpp>

namespace smak::models {
    
#include OATPP_CODEGEN_BEGIN(DTO)

using chess::GameResult;
using chess::GameResultReason;
ENUM(GameResultEnum, v_int32,
    VALUE(WIN,  static_cast<v_int32>(GameResult::WIN )),
    VALUE(LOSE, static_cast<v_int32>(GameResult::LOSE)),
    VALUE(DRAW, static_cast<v_int32>(GameResult::DRAW)),
    VALUE(NONE, static_cast<v_int32>(GameResult::NONE))
);

ENUM(GameResultReasonEnum, v_int32,
    VALUE(CHECKMATE,             static_cast<v_int32>(GameResultReason::CHECKMATE)),
    VALUE(STALEMATE,             static_cast<v_int32>(GameResultReason::STALEMATE)),
    VALUE(INSUFFICIENT_MATERIAL, static_cast<v_int32>(GameResultReason::INSUFFICIENT_MATERIAL)),
    VALUE(FIFTY_MOVE_RULE,       static_cast<v_int32>(GameResultReason::FIFTY_MOVE_RULE)),
    VALUE(THREEFOLD_REPETITION,  static_cast<v_int32>(GameResultReason::THREEFOLD_REPETITION)),
    VALUE(NONE,                  static_cast<v_int32>(GameResultReason::NONE))
);

class EvalDTO : public oatpp::DTO {
    DTO_INIT(EvalDTO, DTO)

    DTO_FIELD(Int64, id);
    DTO_FIELD(Int32, ply);
    DTO_FIELD(Enum<GameResultEnum>, result);
    DTO_FIELD(Enum<GameResultReasonEnum>, reason);

    DTO_FIELD_INFO(forced_mate) {
        info->required = false;
    }
    DTO_FIELD(Boolean, forced_mate);

    DTO_FIELD(Float64, pawn_eval);
    // DTO_FIELD_INFO(bestmove){
    //     info->description = "Engine's bestmove. square_from in upper 8 bits, square_to in lower 8 bits.";
    // }
    DTO_FIELD(String, bestmove);

    DTO_FIELD_INFO(ponder) {
        info->required = false;
    }
    DTO_FIELD(String, ponder);

    DTO_FIELD(String, move);

};


#include OATPP_CODEGEN_END(DTO)


}

#endif
