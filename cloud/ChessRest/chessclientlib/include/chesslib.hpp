#pragma once

#ifndef SMAK_CHESSLIB_HPP
#define SMAK_CHESSLIB_HPP

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

class SmakPgnVisitor : public chess::pgn::Visitor {
private:
    std::vector<chess::Move> _moves_out{};
    chess::Board _board;
public:
    virtual ~SmakPgnVisitor() {}
    SmakPgnVisitor(){}

    void startPgn();

    void header(std::string_view key, std::string_view value);

    void startMoves();

    void move(std::string_view move, std::string_view comment);

    void endPgn();

    std::vector<chess::Move> getMoves() { return _moves_out; }
};

class PgnParser {
private:
    bool valid = false;
    chess::pgn::StreamParserError _err;
    SmakPgnVisitor svis;
    std::istream& _stream;
public:
    PgnParser(std::istream& pgn_stream);
    std::optional<std::vector<chess::Move>> getMoves();
    void parse();
};


}

#endif // SMAK_CHESSLIB_HPP
