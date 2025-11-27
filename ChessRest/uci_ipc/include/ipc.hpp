#pragma once 

#ifndef IPC_HPP
#define IPC_HPP

#include <uci_commands.hpp>
#include <chess.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/process/v2/process.hpp>
#include <boost/process/v2/popen.hpp>

#include <vector>
#include <exception>
#include <string>
#include <string_view>
#include <optional>
#include <utility>
#include <variant>

/**
 * Class that holds the evaluation of the position, and the "bestmove AxBy ponder CzDw" string from the engine. 
 * 
 */
class Evaluation {
    double m_eval; 
    std::string bestmove_ponder;
    chess::Move m_bestmove;
    chess::Move m_ponder;
    chess::Color m_color_to_move;
    bool m_mate = false;
    int m_matec = 0;
    chess::Color m_has_mate;
    bool m_mate_played = false;
public:
    /**
     * @brief Empty constructor.
     * 
     */
    Evaluation(){}
    /**
     * @brief Simple constructor.
     * 
     * @param evaluation The evaluation of the position.
     * @param suggested_moves The engine's "bestmove" and "ponder".
     */
    Evaluation(double evaluation, std::string suggested_moves) : m_eval(evaluation), bestmove_ponder(suggested_moves) {};
    /**
     * @brief Getter for the engine's "bestmove".
     * @return chess::Move The engine's best move.
     */
    chess::Move getBestmove() { return m_bestmove; }
    /**
     * @brief Getter for the engine's "ponder".
     * @return chess::Move The engine's ponder.
     */
    chess::Move getPonder() { return m_ponder; }
    /**
     * @brief Getter for the evaluation of the position.
     * @return double The evaluation of the position in pawns.
     */
    double getEval() { return m_eval; }
    /**
     * @brief Updates the evaluation of the position.
     * 
     * @param color_to_move Whether it is white's turn to move.
     * @param eval The evaluation of the position in pawns.
     * @param best The engine's recommended best move.
     * @param ponder The engine's pondered move.
     * @param is_mate Whether checkmate is on the board.
     * @param has_mate The color of the side that has checkmate.
     * @param moves_to_mate Amount of moves to checkmate.
     */
    void update(chess::Color color_to_move, double eval, chess::Move best, chess::Move ponder, bool is_mate = false, chess::Color has_mate = chess::Color::NONE, int moves_to_mate = 0);
    /**
     * @brief Returns the evaluation in a human readable(ish) string.
     * 
     * @return The evaluation as a string.
     */
    const std::string to_string();
    /**
     * @brief Construct a string from an evaluation. Same as <tt>to_string()</tt>.
     * 
     * @return The evaluation as a string.
     */
    operator std::string();
    /**
     * @brief Setter for the engine's given best move.
     * @param move The move to set as the best move.
     */
    void setBestmove(chess::Move move);
    /**
     * @brief Setter for the engine's given ponder.
     * @param move The move to set as ponder.
     */
    void setPonder(chess::Move move);
    /**
     * @brief Set the engine's evaluation in pawns.
     * 
     * @param eval The evaluation to set.
     */
    void setEval(double eval)       { m_eval = eval;        }
    /**
     * @brief Set whether checkmate has been played.
     * 
     * @param mate Whether checkmate has been played. 
     */
    void setMatePlayed(bool mate)   { m_mate_played = mate; }
    /**
     * @brief Get whether checkmate has been played.
     * 
     * @return true Checkmate has been played.
     * @return false Checkmate has not been played.
     */
    bool getMatePlayed()            { return m_mate_played; }
    void setMateCount(size_t mc)    { m_matec = mc;         }
    size_t getMateCount()           { return m_matec;       }
    void setMate(bool mate)         { m_mate = mate;        }
    bool isMate()                   { return m_mate;        }
};

/** 
 * @brief Class for starting and communicating with UCI compliant chess engines. 
 * @note While this is supposed to work with any UCI engine, it has only been tested with Stockfish.  
 */
class EngineWhisperer {
private:
    std::string path_to_engine_executable;
    std::string current_position_fen;
    boost::asio::io_context io;
    boost::process::v2::popen engine_proc;
    chess::Board m_board;
    bool m_white_to_move = true;
    size_t m_depth = 20;
    size_t m_search_timeout = 10;
    size_t m_write_timeout = 5;
    size_t m_read_timeout = 5;
    Evaluation m_eval;

    /**
     * @brief Issue the UCI command "isready" to the engine.
     * 
     * @return true if the engine responded with "readyok".
     * @return false if the engine did not respond within @p m_write_timeout + @p m_read_timeout seconds.
     */
    bool check_engine_isready();
    /**
     * @brief Write a command to the engine process' standard input.
     * 
     * @param command The command to give the engine.
     * @param timeout Timeout in seconds of the write.
     * @return size_t The amount of bytes written.
     */
    size_t write_engine_with_timeout(std::string_view command, size_t timeout = 5);
    /**
     * @brief Read from the engine's output until a specific string is matched.
     * 
     * @param search_string The string to match.
     * @param timeout The maximum time for the read in seconds.
     * @return std::string The entire string read from the engine.
     */
    std::string read_engine_with_timeout(std::string_view search_string, size_t timeout = 5);

    /**
     * @brief Equivalent to calling write_engine_with_timeout() followed by read_engine_with_time_out()
     * @details The total timeout of the function will be two times the given @p timeout parameter. 
     * The function can return early with an empty string, and only the amount of bytes sent in case of an error. 
     * The caller should check if the string is empty on return.
     * @param command The command to write to the engine.
     * @param search_string The string to match in the read operation.
     * @param timeout The timeout of read/write operations in seconds.
     * @return std::pair<size_t, std::string> A pair of the return value of write_engine_with_timeout and read_engine_with_timeout.
     */
    std::pair<size_t, std::string> write_and_read_with_timeout(std::string_view command, std::string_view search_string, size_t timeout = 5);
    std::optional<std::variant<std::pair<chess::Move, chess::Move>, chess::Move>> extractBestmoveFromRegex(std::string_view input);
    std::optional<std::variant<double, size_t>> extractEvalFromRegex(std::string_view input);

public:
    /**
     * @brief Constructor for @p EngineWhisperer. Attempts to launch the engine given in @p engine_path .
     * @param engine_path The path to the executable of engine.
     * If the path is not absolute, it will be searched for in the program's environment.
     * @throws engine_not_launched_exception thrown if the given engine cannot be launched.
     */
    EngineWhisperer(std::string engine_path);
    /**
     * @brief Kill the engine process if it still is running.
     * 
     * First attempts a UCI "quit" command. If this fails it kills the process.
     * 
     */
    ~EngineWhisperer();
    /**
     * @brief Issues the "uci" command to the running engine.
     * @returns true if "uciok" is received, false if not.
     * @throws engine_not_launched_exception thrown if called when the engine process is closed.
     * 
     * Explicitly issues "ucinewgame" and "position startpos" commands to the engine if "uciok" is received.
     */
    bool start_uci();
    /**
     * @brief Resets the engine with the "ucinewgame" command.
     * @throws engine_not_launched_exception if the engine process is not running. 
     */
    void new_game();
    /** 
     * @brief Issues a "position fen" command to the engine with the given string.
     * @param fen The FEN string to set position as.
     * @pre @p fen must be a valid FEN-string.
     * @returns true if @p fen is valid and no error is returned by the engine, otherwise false.
     */
    bool set_position(std::string_view fen);
    /**
     * @fn bool make_moves(chess::Move move)
     * @brief Make a move from the current position. Convenience overload for <tt>make_moves(std::vector<chess::Move>& moves)</tt>.
     * @par Side Effects: 
     * Updates the private members @p current_position_fen and @p m_board to the position after the given move.
     * @param move The move to be made.
     * @todo Find a proper return on error.
     */
    bool make_moves(chess::Move move);
    /**
     * @fn bool make_moves(std::vector<chess::Move>& moves)
     * @brief Make several moves from the current position.
     * @par Side Effects:
     * Updates the private members @p current_position_fen and @p m_board to the position after the given moves.
     * @param moves The moves to be made.
     * @todo Find a proper return on error.
     */
    bool make_moves(std::vector<chess::Move>& moves);
    /**
     * @brief Get the evaluation of the current position in pawns.
     * 
     * @return double
     */
    double getPositionEval();

    /**
     * @brief Get an optional Evaluation from a FEN string without any context of previous moves.
     * 
     * @param fen The position to get the evaluation from.
     * @return std::optional<Evaluation> 
     */
    std::optional<Evaluation> naive_eval_from_position(std::string_view fen);
};
/**
* @brief Exception thrown if operations that expect a running engine are done while the engine hasn't launched.
* 
* @details Constructed with a string to supply to @p #what for more flexibility across the entire class.
*/
class engine_not_launched_exception : std::exception {
    std::string message;
public:
    /**
     * @brief Constructor that takes any message to give to @p #what .
     * 
     * @param msg 
     */
    engine_not_launched_exception(std::string msg) : message(msg){}

    /**
     * @brief Get a description of what went wrong.
     */
    const char * what() const noexcept override {
        return message.c_str();
    }
};
/**
 * @brief Exception thrown if the engine does not reply with "uciok" to the "uci" command.
 * 
 * This can be either due to the engine answering with an error, or the engine not replying with "uciok" within a certain timeframe.
 */
class engine_no_uci_exception : std::exception {
public:
    /**
     * @brief Says that no UCI is available.
     * 
     */
    const char * what() const noexcept override {
        return "No UCI available.";
    }
};

#endif
