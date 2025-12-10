#pragma once
#ifndef SMAK_GAMEDTO_HPP
#define SMAK_GAMEDTO_HPP

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

namespace smak { namespace models {

/* GameDTO BEGIN */
#include OATPP_CODEGEN_BEGIN(DTO)

// Enum values are mapped to strings, with the string being the literal variable name. E.g. WHITE_WIN has an associated string "WHITE_WIN".
// The names MUST match the strings of the corresponding value in GameStateEnum.java
ENUM(GameStateEnum, v_int32,
    VALUE(WHITE_WIN,        0),
    VALUE(BLACK_WIN,        1),
    VALUE(DRAW_AGREED,      2),
    VALUE(DRAW_REPETITION,  3), 
    VALUE(DRAW_FIFTY_MOVES, 4),
    VALUE(WHITE_TO_MOVE,    5),
    VALUE(BLACK_TO_MOVE,    6),
    VALUE(NOT_STARTED,      7),
    VALUE(ERROR,            8)
);
    

class GameDTO : public oatpp::DTO {
    

    DTO_INIT(GameDTO, DTO)

    DTO_FIELD(Int64, id);

    DTO_FIELD(Enum<GameStateEnum>, gamestate);
    
    DTO_FIELD(String, gamestart);
    
};

#include OATPP_CODEGEN_END(DTO)

}}

#endif
