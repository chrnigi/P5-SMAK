#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/process/v2.hpp>
#include <cstdint>
#include <exception>
#include <memory>
#include <string>
#include <string_view>

/**
 * Class that holds the evaluation of the position, and the "bestmove AxBy ponder CzDw" string from the engine. 
 * 
 */
class alignas(64) Evaluation {
    double eval;
    std::string bestmove_ponder;
public:
    /**
     * @brief Simple constructor.
     * 
     * @param evaluation The evaluation of the position.
     * @param suggested_moves The engine's "bestmove" and "ponder".
     */
    Evaluation(double evaluation, std::string suggested_moves) : eval(evaluation), bestmove_ponder(suggested_moves) {};
    
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
    double getEval() { return eval; }


};

/** @brief Class for starting and communicating with UCI compliant chess engines. */
class EngineWhisperer {
private:
    std::string path_to_engine_executable;
    std::string current_position_fen;
    boost::asio::io_context io;
    boost::process::v2::popen engine_proc;
    
public:

    /**
     * @brief Constructor for @p EngineWhisperer. Attempts to launch the engine given in @p engine_path .
     * @param engine_path The path to the executable of engine.
     * If the path is not absolute, it will be searched for in the program's environment.
     * @throws engine_not_launched_exception thrown if the given engine cannot be launched.
     */
    EngineWhisperer(std::string engine_path);

    /**
     * @brief Issues the "uci" command to the running engine.
     * @returns true if "uciok" is received, false if not.
     * @throws engine_not_launched_exception thrown if called when the engine process is closed.
     * 
     * Explicitly issues "position startpos" command to the engine.
     */
    bool start_uci();

    /**
     *  @brief Resets the engine with the "ucinewgame" command.
     */
    void new_game();

    /** 
     * @brief Issues a "position fen" command to the engine with the given string.
     * @param fen Must be a valid FEN-string.
     * @returns true if @p fen is valid an no error is returned by the engine, otherwise false.
     */
    Evaluation set_position(std::string_view fen);
    
    /**
     * @brief Make a move from the current position.
     * @par Side Effects: 
     * Updates the private member @p current_position_fen to the position after the given move.
     * @param to The square which the move is made from.
     * @param from The square which the move goes to.
     */
    Evaluation make_move(int16_t to, int16_t from);

    /**
     * @brief Kill the engine process if it still is running.
     * 
     * First attempts a UCI "quit" command. If this fails it kills the process.
     * 
     */
    ~EngineWhisperer();

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
    /**
     * @brief 
     * 
     */
    const char * what() const noexcept override {
        return "No UCI available.";
    }
};
