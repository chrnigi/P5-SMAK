#pragma once

#ifndef UCI_COMMANDS_HPP
#define UCI_COMMANDS_HPP

#include <vector>
#include <chess.hpp>
#include <string_view>

/**
 * @brief UCI command factory class thingy.
 * @note The commands are all wrapped in "\n <command>\n", due to how Stockfish interacts with its input pipe.
 */
struct UCIcommand {
private:
    UCIcommand() {}

public:
    /**
    * @brief Possible commands returned from the engine.
    * 
    */
    struct EngineCommands {
        /**
         * @brief The UCI engine response to the "isready" command.
         * 
         * @return constexpr "readyok"
         */
        static constexpr std::string_view readyok() { return "readyok"; }
        /**
         * @brief The UCI engine response to the "uci" command.
         * 
         * @return constexpr std::string_view 
         */
        static constexpr std::string_view uciok()   { return "uciok";   }
    };
    /**
     * @fn std::string create_position_command(std::string_view fen)
     * @brief Create a UCI command in the form of "position fen <fen>".
     * 
     * @param fen The FEN string to set as position.
     * @return std::string
     */
    static const std::string create_position_command(std::string_view fen);
    /**
     * @fn std::string create_position_command(std::string_view fen, std::vector<chess::Move>& moves)
     * @brief Create a UCI command in the form of "position fen <fen> <moves>".
     * 
     * @param fen The FEN string to start the moves from.
     * @param moves Moves to play from the position given in the FEN string.
     * @return std::string 
     */
    static const std::string create_position_command(std::string_view fen, std::vector<chess::Move>& moves);
    /**
     * @fn std::string append_moves_to_position_command(std::string_view pos_command, chess::Move move)
     * @brief Convenience overload for <tt>append_moves_to_position_command(std::string pos_command, std::vector<chess::Move> moves)</tt>
     * 
     * @param pos_command The UCI position command.
     * @param move The move to append to the given position command.
     * @return std::string
     */
    static const std::string append_moves_to_position_command(std::string_view pos_command, chess::Move move);
    /**
     * @fn std::string append_moves_to_position_command(std::string_view pos_command, std::vector<chess::Move>& moves)
     * @brief Appends a list of moves to an already existing UCI position command.
     * 
     * @param pos_command The UCI position command.
     * @param moves @p std::vector of moves to append to the command.
     * @return std::string 
     */
    static const std::string append_moves_to_position_command(std::string_view pos_command, std::vector<chess::Move>& moves);
    /**
     * @brief Create a UCI "go depth" command with a given depth of @p depth
     * 
     * @param depth The desired search depth.
     * @return std::string 
     *
     * @details Depth is set at a default of 20. This is chosen arbitrarily.
     */
    static const std::string create_go_depth_command(unsigned int depth = 20);
    /**
     * @brief Get the UCI "stop" command.
     * 
     * @return constexpr "\n stop\n"
     */
    static constexpr std::string_view stop()                { return "\n stop\n";               }
    /**
     * @brief Get the UCI "quit" command.
     * 
     * @return constexpr "\n quit\n"
     */
    static constexpr std::string_view quit()                { return "\n quit\n";               }
    /**
     * @brief Get the UCI "ucinewgame" command.
     * 
     * @return constexpr "\n ucinewgame\n"
     */
    static constexpr std::string_view ucinewgame()          { return "\n ucinewgame\n";         }
    /**
     * @brief Get the UCI "ponderhit" command.
     * 
     * @return constexpr "\n ponderhit\n"
     */
    static constexpr std::string_view ponderhit()           { return "\n ponderhit\n";          }
    /**
     * @brief Get the UCI command "position startpos"
     * 
     * @return constexpr "\n position startpos\n" 
     */
    static constexpr std::string_view position_startpos()   { return "\n position startpos\n";  }
    /**
     * @brief Get the UCI "isready" command.
     * 
     * @return constexpr "\n isready\n" 
     */
    static constexpr std::string_view isready()             { return "\n isready\n";            }
    /**
     * @brief Get the UCI "uci" command.
     * 
     * @return constexpr "\n uci\n"
     */
    static constexpr std::string_view uci()                 { return "\n uci\n";                }
};

#endif
