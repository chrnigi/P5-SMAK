#include <chess.hpp>
#include <ipc.hpp>
#include <fmt/compile.h>
#include <fmt/core.h>
#include <string>
#include <string_view>
#include <cmath>

void Evaluation::update(chess::Color color_to_move, double eval, chess::Move best, chess::Move ponder, bool is_mate, chess::Color has_mate, int moves_to_mate) {
    m_color_to_move = color_to_move;
    m_eval          = eval;
    m_bestmove      = best;
    m_ponder        = ponder;
    m_mate          = is_mate;
    m_has_mate      = has_mate;
    m_matec         = moves_to_mate;
}

const std::string Evaluation::to_string() {
        
    if (m_mate) {
        return fmt::format(FMT_COMPILE("{} has checkmate in {} moves."), m_has_mate.longStr(), m_matec);
    }
    std::string_view better = std::lround(m_eval) > 0 ? "White" : "Black";

    std::floorf(m_eval*100)/100;

    return fmt::format(FMT_COMPILE("{} is better. Position is {:+}"), better, m_eval);
}

Evaluation::operator std::string() {
    return to_string();
}
