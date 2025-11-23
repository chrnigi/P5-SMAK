#include "chess.hpp"
#include <string>
#include <string_view>
#include <uci_commands.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/compile.h>
#include <fmt/ranges.h>
#include <vector>



const std::string UCIcommand::newlineify(std::string str) {
    return fmt::format(FMT_COMPILE("\n {}\n"), str);
} 

const std::string UCIcommand::create_position_command(std::string_view fen) {
    return fmt::format(FMT_COMPILE("\n position fen {}\n"), fen);
}

const std::string UCIcommand::create_position_command(std::string_view fen, std::vector<chess::Move>& moves) {    
    std::vector<std::string> mvs;
    mvs.reserve(moves.size());

    for (auto m : moves) {
        mvs.push_back(chess::uci::moveToUci(m));
    }
    return fmt::format(FMT_COMPILE("\n position fen {} moves {}\n"), fen, fmt::join(mvs, " "));
}

const std::string UCIcommand::append_moves_to_position_command(std::string_view pos_command, chess::Move move) {
    return append_moves_to_position_command(pos_command, {move});
}

const std::string UCIcommand::append_moves_to_position_command(std::string_view pos_command, std::vector<chess::Move>& moves) {
    std::vector<std::string> mvs;
    mvs.reserve(moves.size());


    for (auto m : moves) {
        mvs.push_back(chess::uci::moveToUci(m));
    }

    pos_command.remove_suffix(1);
    pos_command.remove_prefix(2);
    return fmt::format(FMT_COMPILE("\n {} moves {}\n"), pos_command, fmt::join(mvs, " "));
}

const std::string UCIcommand::create_go_depth_command(unsigned int depth) {
    return fmt::format(FMT_COMPILE("\n go depth {}\n"), depth);
}
