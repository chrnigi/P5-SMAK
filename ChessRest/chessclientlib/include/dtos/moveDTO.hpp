#pragma once
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <chess.hpp>
#ifndef SMAK_MOVEDTO_HPP
#define SMAK_MOVEDTO_HPP

namespace smak { namespace models {


#include OATPP_CODEGEN_BEGIN(DTO)

ENUM(PiecesEnum, v_int32,
    VALUE(KING, 0,  "K"),
    VALUE(QUEEN, 1, "Q"),
    VALUE(ROOK, 2,  "R"),
    VALUE(BISHOP, 3,"B"),
    VALUE(KNIGHT, 4,"N"),
    VALUE(PAWN, 5,  "P"),
    VALUE(NONE, 6,  "Z")
)

// chess::Move values are 1 << 14 etc. so to get clean values in enum they are bitshifted back
// The strings associated with these enum values MUST match the ones in MoveTypeEnum.java, else we run into JSON parsing issues. See below.
ENUM(MoveType, v_int32,
    VALUE(PROMOTION,    chess::Move::PROMOTION  >> 14, "promotion"),
    VALUE(ENPASSANT,    chess::Move::ENPASSANT  >> 14, "enpassant"),
    VALUE(CASTLING,     chess::Move::CASTLING   >> 14, "castling"),
    VALUE(NORMAL,       chess::Move::NORMAL,           "normal"),
    VALUE(NULLMOVE,     chess::Move::NULL_MOVE  >> 4,  "nullmove")
)

// Java version:
/*********************************/
// public enum MoveTypeEnum {
//     PROMOTION("promotion"),
//     ENPASSANT("enpassant"),
//     CASTLES("castling"),
//     NORMAL("normal"),
//     NULLMOVE("nullmove");
// }
/*********************************/


class MoveDTO : oatpp::DTO {

    DTO_INIT(MoveDTO, DTO)

    DTO_FIELD(Int64, id);
    DTO_FIELD(Int32, ply_number);   
    DTO_FIELD(Enum<MoveType>, move_type);
    DTO_FIELD(Enum<PiecesEnum>, piece_moved);
    DTO_FIELD(Enum<PiecesEnum>, piece_captured);
    DTO_FIELD(Int16, from_square);  
    DTO_FIELD(Int16, to_square);    

};

#include OATPP_CODEGEN_END(DTO)

} }

#endif
