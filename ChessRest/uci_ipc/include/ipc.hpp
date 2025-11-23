#pragma once 

#ifndef IPC_HPP
#define IPC_HPP

#include <uci_commands.hpp>
#include <chess.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/process/v2.hpp>

#include <vector>
#include <exception>
#include <string>
#include <string_view>

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
     * @return Returned as a @p std::string_view to avoid new allocations.
     */
    std::string_view bestmove();
    /**
     * @brief Getter for the engine's "ponder".
     * @return Returned as a @p std::string_view to avoid new allocations.
     */
    std::string_view ponder();
    /**
     * @brief Getter for the evaluation of the position.
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
     * @brief Construct a string from an evaluation. Same as <tt>to_string()</tt>
     * 
     * @return The evaluation as a string.
     */
    operator std::string();
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
    int m_depth = 20;
    int search_timeout = 10;
    Evaluation m_eval;
    
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
     * Explicitly issues "ucinewgame" and "position startpos" commands to the engine.
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
     * @brief Get the evaluation of the current position.
     * 
     * @return double
     */
    double getPositionEval();
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
