#include <chesslib.hpp>
#include <chess.hpp>
#include <vector>


namespace smak::parsing {

GameOfFens::GameOfFens(std::vector<SimpleMoveDTO>& moves) {

    board.setFen(chess::constants::STARTPOS);

    positions.reserve(moves.size()+1);
    positions.push_back(board.getFen());

    chess::Move mv;

    for (auto& move : moves) {
        
        switch (move.type) {

        case SimpleMoveDTO::CASTLES:
            mv = chess::Move::make<chess::Move::CASTLING>(move.from, move.to);
            break;
        case SimpleMoveDTO::PROMOTION:
            mv = chess::Move::make<chess::Move::PROMOTION>(move.from, move.to, chess::PieceType::QUEEN); // We only support queen promotion.
            break;
        case SimpleMoveDTO::ENPASSANT:
            mv = chess::Move::make<chess::Move::ENPASSANT>(move.from, move.to);
            break;
        case SimpleMoveDTO::NORMAL:
            mv = chess::Move::make<chess::Move::NORMAL>(move.from, move.to);
            break;
        case SimpleMoveDTO::NULLMOVE:
            mv = chess::Move::make<chess::Move::NULL_MOVE>(move.from, move.to);
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
