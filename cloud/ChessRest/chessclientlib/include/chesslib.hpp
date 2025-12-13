#pragma once

#ifndef SMAK_CHESSLIB_HPP
#define SMAK_CHESSLIB_HPP

#include <fstream>
#include <string_view>
#include <string>
#include <vector>
#include <chess.hpp>
#include <clientlib.hpp>

namespace smak::parsing {




class GameOfFens {
private:
    std::vector<std::string> positions;
    
    chess::Board board;

public:

    GameOfFens(oatpp::Vector<oatpp::Object<models::MoveDTO>>& moves);

    size_t getPositionCount() { return positions.size(); }
    std::string getPositionByPly(size_t ply_number);
    std::vector<std::string> getAllPositions();

}; 

class SmakPgnVisitor : chess::pgn::Visitor {
private:
    std::vector<chess::Move>& _moves_out;

    virtual ~SmakPgnVisitor() {}

    void startPgn();

    void header(std::string_view key, std::string_view value);

    void startMoves();

    void move(std::string_view move, std::string_view comment);

    void endPgn();

public:
    SmakPgnVisitor(std::vector<chess::Move>& moves_vec);
};

class PgnParser {
private:
    std::vector<chess::Move> moves {};
    SmakPgnVisitor svis{moves};
    std::ifstream pgn_filestream;
public:
    std::optional<std::vector<chess::Move>>& getMoves();
};


}

#endif // SMAK_CHESSLIB_HPP
