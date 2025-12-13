#include <models.hpp>
#include <oatpp/core/Types.hpp>
#include <chesslib.hpp>
#include <chess.hpp>
#include <string>
#include <string_view>
#include <vector>


/* GameOfFens */
namespace smak::parsing {

GameOfFens::GameOfFens(oatpp::Vector<oatpp::Object<models::MoveDTO>>& moves) {

    board.setFen(chess::constants::STARTPOS);

    positions.reserve(moves->size()+1);
    positions.push_back(board.getFen());

    auto getVal = [&](oatpp::Int16 wrapper) {
        return wrapper.getValue(0);
    };

    for (auto& move : *moves) {
        
        const chess::Move mv = [&]() {
            switch (*(move->move_type.get())) {

            case models::MoveType::CASTLING:
                return chess::Move::make<chess::Move::CASTLING>(
                    getVal(move->from_square), 
                    getVal(move->to_square));
                break;
            case models::MoveType::PROMOTION:
                return chess::Move::make<chess::Move::PROMOTION>(
                    getVal(move->from_square), 
                    getVal(move->to_square),
                    chess::PieceType::QUEEN); // We only support queen promotion. 
                break;
            case models::MoveType::ENPASSANT:
                return chess::Move::make<chess::Move::ENPASSANT>(
                    getVal(move->from_square), 
                    getVal(move->to_square));
                break;
            case models::MoveType::NORMAL:
                return chess::Move::make<chess::Move::NORMAL>(
                    getVal(move->from_square),
                    getVal(move->to_square));
                break;
            case models::MoveType::NULLMOVE:
            default:
                return chess::Move::make<chess::Move::NULL_MOVE>(
                    getVal(move->from_square),
                    getVal(move->to_square));
                break;
            }
        }();


        board.makeMove(mv);
        positions.push_back(board.getFen());
    }
}

std::string GameOfFens::getPositionByPly(size_t ply_number) {
    if (positions.size() < ply_number)
        return std::string();

    std::string out = positions.at(ply_number);
    return out;
}

std::vector<std::string> GameOfFens::getAllPositions() {
    return positions;
}

}

/* SmakPgnVisitor*/
namespace smak::parsing {

SmakPgnVisitor::SmakPgnVisitor(std::vector<chess::Move>& moves_vec) : _moves_out(moves_vec) {

}

void SmakPgnVisitor::startPgn() {

}

void SmakPgnVisitor::header(std::string_view key, std::string_view value) {
    /* too complicated for web interface, so ignored*/
}

void SmakPgnVisitor::startMoves() {

}

void SmakPgnVisitor::move(std::string_view move, std::string_view comment) {

}

void SmakPgnVisitor::endPgn() {

}

}

/* PgnParser */
namespace smak::parser {



}
