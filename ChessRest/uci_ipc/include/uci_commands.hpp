#pragma once

#include <vector>
#ifndef UCI_COMMANDS_HPP
#define UCI_COMMANDS_HPP
#include <chess.hpp>
#include <string_view>

/**
 * @brief UCI command factory class thingy.
 * @note The commands are all wrapped in "\n <command>\n", due to how Stockfish interacts with its input pipe.
 */
struct UCIcommand {

private:
    const std::string newlineify(std::string str);
    UCIcommand() {}
public:

    /**
     * @brief The base UCI commands without any options.
     * 
     */
    struct base_commands {
        static constexpr std::string_view quit       = "quit";
        static constexpr std::string_view uci        = "uci";
        static constexpr std::string_view setoption  = "setoption";
        static constexpr std::string_view position   = "position";
        static constexpr std::string_view ucinewgame = "ucinewgame";
        static constexpr std::string_view isready    = "isready";
        static constexpr std::string_view go         = "go";
        static constexpr std::string_view stop       = "stop";
        static constexpr std::string_view ponderhit  = "ponderhit";
    };

    /**
     * @fn std::string create_position_command(std::string_view fen)
     * @brief Create a UCI command in the form of "position <fen>".
     * 
     * @param fen The FEN string to set as position.
     * @return std::string
     */
    static const std::string create_position_command(std::string_view fen);
    /**
     * @fn std::string create_position_command(std::string_view fen, std::vector<chess::Move> moves)
     * @brief Create a UCI command in the form of "position <fen> <moves>".
     * 
     * @param fen The FEN string to start the moves from.
     * @param moves Moves to play from the position given in the FEN string.
     * @return std::string 
     */
    static const std::string create_position_command(std::string_view fen, std::vector<chess::Move> moves);
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
     * @fn std::string append_moves_to_position_command(std::string_view pos_command, std::vector<chess::Move> moves)
     * @brief Appends a list of moves to an already existing UCI position command.
     * 
     * @param pos_command The UCI position command.
     * @param moves @p std::vector of moves to append to the command.
     * @return std::string 
     */
    static const std::string append_moves_to_position_command(std::string_view pos_command, std::vector<chess::Move> moves);
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
     * @brief Create the UCI command "position startpos"
     * 
     * @return constexpr "\n position startpos\n" 
     */
    static constexpr std::string_view position_startpos()   { return "\n position startpos\n";  }
};

#endif
