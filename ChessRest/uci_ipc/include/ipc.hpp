#include <cstdint>
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

/** Class for starting and communicating with UCI compliant chess engines. */
class EngineWhisperer {
private:
    std::string path_to_engine_executable;
    std::string current_position_fen;
    
public:

    /**
     * @brief Constructor for @p EngineWhisperer. 
     * @param engine_path The path to the executable of engine.
     * If the path is not absolute, it will be searched for in the program's environment.
     */
    EngineWhisperer(std::string engine_path);

    /**
     * @brief Starts a uci compliant engine, and issues the "uci" command to it.
     * @returns true if "uciok" is received, false if not.
     */
    bool start_engine();

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
     * - First attempt a UCI "quit" command.
     * - Else kill the process.
     * 
     */
    ~EngineWhisperer();


};
