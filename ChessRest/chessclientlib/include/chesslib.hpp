#pragma once
#ifndef SMAK_CHESSLIB_HPP
#define SMAK_CHESSLIB_HPP
#include <string>
#include <vector>
#include <chess.hpp>

namespace smak::parsing {

class SimpleMoveDTO {
public:
    enum MoveType {
        PROMOTION = chess::Move::PROMOTION,
        ENPASSANT = chess::Move::ENPASSANT,
        CASTLES = chess::Move::CASTLING,
        NORMAL = chess::Move::NORMAL,
        NULLMOVE = chess::Move::NULL_MOVE >> 4,

    } type;

    bool is_promotion = false;
    bool is_capture = false;
    bool is_enpassant = false;
    bool is_normal = true;
    int16_t from;
    int16_t to;

};


class GameOfFens {
private:
    std::vector<std::string> positions;
    
    chess::Board board;

public:

    GameOfFens(std::vector<SimpleMoveDTO>& moves);

    size_t getPositionCount() { return positions.size(); }
    std::string getPositionByPly(size_t ply_number);
    std::vector<std::string> getAllPositions();

}; 



}

#endif // SMAK_CHESSLIB_HPP