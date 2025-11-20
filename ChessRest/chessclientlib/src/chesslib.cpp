#include "dtos/moveDTO.hpp"
#include "oatpp/core/Types.hpp"
#include <chesslib.hpp>
#include <chess.hpp>
#include <vector>


namespace smak::parsing {

GameOfFens::GameOfFens(oatpp::Vector<oatpp::Object<models::MoveDTO>>& moves) {

    board.setFen(chess::constants::STARTPOS);

    positions.reserve(moves->size()+1);
    positions.push_back(board.getFen());

    chess::Move mv;

    auto getVal = [&](oatpp::Int16 wrapper) {
        return wrapper.getValue(0);
    };

    for (auto& move : *moves) {
        
        switch (*(move->move_type.get())) {

        case models::MoveType::CASTLING:
            mv = chess::Move::make<chess::Move::CASTLING>(
                getVal(move->from_square), 
                getVal(move->to_square));
            break;
        case models::MoveType::PROMOTION:
            mv = chess::Move::make<chess::Move::PROMOTION>(
                getVal(move->from_square), 
                getVal(move->to_square),
                chess::PieceType::QUEEN); // We only support queen promotion. 
            break;
        case models::MoveType::ENPASSANT:
            mv = chess::Move::make<chess::Move::ENPASSANT>(
                getVal(move->from_square), 
                getVal(move->to_square));
            break;
        case models::MoveType::NORMAL:
            mv = chess::Move::make<chess::Move::NORMAL>(
                getVal(move->from_square),
                getVal(move->to_square));
            break;
        case models::MoveType::NULLMOVE:
            mv = chess::Move::make<chess::Move::NULL_MOVE>(
                getVal(move->from_square),
                getVal(move->to_square));
            break;
        }
        board.makeMove(mv);
        positions.push_back(board.getFen());
    }
}

std::string GameOfFens::getPositionByPly(size_t ply_number) {
    std::string out("");
    if (positions.size() < ply_number)
        return out;

    out = positions.at(ply_number);
    return out;
}

std::vector<std::string> GameOfFens::getAllPositions() {
    return positions;
}

}
