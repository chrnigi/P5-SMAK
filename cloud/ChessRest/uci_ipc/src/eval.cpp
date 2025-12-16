#include <plog/Log.h>
#include <chess.hpp>
#include <ipc.hpp>
#include <optional>
#include <utility>
#include <vector>
#include <fmt/compile.h>
#include <fmt/core.h>
#include <fmt/base.h>

/**
 * @todo Set all moves' promotion type to be Queen.
 */
std::vector<std::optional<Evaluation>> EngineWhisperer::getEvalsFromGame(const std::vector<chess::Move>& moves) {
    PLOG_DEBUG << fmt::format(FMT_COMPILE("Getting evals from moves {}"), chess::uci::moveToUci(moves.at(0)));
    using namespace chess;

    if (!engine_proc.running()) {
        throw new engine_not_launched_exception("Engine not running.");
    }
    Board game_board{};

    std::vector<std::optional<Evaluation>> evals_out{};
    evals_out.reserve(moves.size());

    new_game();

    for (auto& m : moves) {
        Movelist legal;
        
        const PieceGenType piecetype = piece_to_piecegen(game_board.at<PieceType>(m.from()));

        movegen::legalmoves<movegen::MoveGenType::ALL>(legal, game_board, piecetype);
        auto it = std::find(legal.begin(), legal.end(), m);

        if (it == legal.end() /* Move was illegal*/) {
            PLOGD << fmt::format(FMT_COMPILE("Illegal move {}."), uci::moveToUci(m));
            evals_out.push_back({});
            game_board.makeNullMove();
            continue;            
        }

        game_board.makeMove(m);
        
        auto ev = naive_eval_from_position(game_board.getFen());

        if(!ev) {
            evals_out.push_back(ev);
            continue;
        }
        
        PLOGD << fmt::format(FMT_COMPILE("{}"), ev->to_string());
        std::pair<GameResultReason, GameResult> result = game_board.isGameOver();
        ev->reason = result.first;
        ev->res = result.second;
        
        if (game_board.sideToMove() == chess::Color::BLACK) {
            ev->m_eval = -(ev->m_eval);
        }
        ev->fen = game_board.getFen();
        evals_out.push_back(ev);

        /* if game_board says we are done, we are done. */
        if (ev->res != GameResult::NONE) {
            break;
        }
    }

    return evals_out;
}
