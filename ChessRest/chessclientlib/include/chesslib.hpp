#pragma once
#include "dtos/moveDTO.hpp"
#include "oatpp/core/Types.hpp"
#ifndef SMAK_CHESSLIB_HPP
#define SMAK_CHESSLIB_HPP
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



}

#endif // SMAK_CHESSLIB_HPP
