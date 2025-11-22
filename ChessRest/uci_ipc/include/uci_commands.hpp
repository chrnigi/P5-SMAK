#pragma once

#ifndef UCI_COMMANDS_HPP
#define UCI_COMMANDS_HPP
#include <chess.hpp>
#include <string_view>
#include <initializer_list>

/**
 * @brief Holds constants related to UCI commands.
 * 
 */
struct UCIcommand {
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
     * @brief Create the UCI command "position startpos"
     * 
     * @return constexpr std::string_view 
     */
    constexpr static std::string_view position_startpos() { return "\n position startpos\n"; };
    
    /**
     * @fn std::string_view create_position_command(std::string_view fen)
     * @brief Create a UCI command in the form of "position <fen>".
     * 
     * @param fen The FEN string to set as position.
     * @return std::string_view 
     */
    static std::string_view create_position_command(std::string_view fen);
    
    /**
     * @fn std::string_view create_position_command(std::string_view fen, std::initializer_list<chess::Move> moves)
     * @brief Create a UCI command in the form of "position <fen> <moves>".
     * 
     * @param fen The FEN string to start the moves from.
     * @param moves Moves to play from the position given in the FEN string.
     * @return std::string_view 
     */
    static std::string_view create_position_command(std::string_view fen, std::initializer_list<chess::Move> moves);

    /**
     * @fn std::string_view create_position_command(std::initializer_list<chess::Move> moves)
     * @brief Create a UCI command in the form of "position <moves>"
     * @param moves Moves to play from the engine's currently stored position.
     * @return std::string_view 
     */
    static std::string_view create_position_command(std::initializer_list<chess::Move> moves);

    /**
     * @fn std::string_view append_moves_to_position_command(std::string pos_command, chess::Move move)
     * @brief Convenience overload for <tt>append_moves_to_position_command(std::string pos_command, std::initializer_list<chess::Move> moves)</tt>
     * 
     * @param pos_command The UCI position command.
     * @param move The move to append to the given position command.
     * @return std::string_view 
     */
    static std::string_view append_moves_to_position_command(std::string pos_command, chess::Move move);

    /**
     * @fn std::string_view append_moves_to_position_command(std::string pos_command, std::initializer_list<chess::Move> moves)
     * @brief Appends a list of moves to an already existing UCI position command.
     * 
     * @param pos_command The UCI position command.
     * @param moves Initializer list of moves to append to the command.
     * @return std::string_view 
     */
    static std::string_view append_moves_to_position_command(std::string pos_command, std::initializer_list<chess::Move> moves);

    /**
     * @brief Create a UCI "go depth" command with a given depth of @p depth
     * 
     * @param depth The desired search depth.
     * @return std::string_view 
     */
    static std::string_view create_go_depth_command(unsigned int depth);

    /**
     * @brief Get the UCI "stop" command.
     * 
     * @return constexpr std::string_view 
     */
    static constexpr std::string_view stop()        { return "\n stop\n"; }
    /**
     * @brief Get the UCI "quit" command.
     * 
     * @return constexpr std::string_view 
     */
    static constexpr std::string_view quit()        { return "\n quit\n"; }
    /**
     * @brief Get the UCI "ucinewgame" command.
     * 
     * @return constexpr std::string_view 
     */
    static constexpr std::string_view ucinewgame()  { return "\n ucinewgame\n"; }
    /**
     * @brief Get the UCI "ponderhit" command.
     * 
     * @return constexpr std::string_view 
     */
    static constexpr std::string_view ponderhit()   { return "\n ponderhit\n"; }

};

#endif
