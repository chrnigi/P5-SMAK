#include <chess.hpp>
#include <ipc.hpp>
#include <uci_commands.hpp>

#include <boost/asio.hpp>
#include <boost/process/v2/process.hpp>
#include <boost/process/popen.hpp>
#include <boost/regex.hpp>
#include <boost/process/v2/environment.hpp>

#include <fmt/core.h>
#include <fmt/compile.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <plog/Log.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Formatters/TxtFormatter.h>

#include <sstream>
#include <string_view>
#include <string>
#include <chrono>
#include <utility>
#include <variant>
#include <optional>
#include <vector>
#include <algorithm>


namespace asio = boost::asio;
namespace proc = boost::process::v2;
using namespace asio::buffer_literals;
using TwoMoves = std::pair<chess::Move, chess::Move>; 

EngineWhisperer::EngineWhisperer(std::string engine_path) :
    path_to_engine_executable(proc::environment::find_executable(engine_path).string()),
    engine_proc(io, path_to_engine_executable, {})
{
    m_board.setFen(chess::constants::STARTPOS);
    plog::init<plog::TxtFormatter>(plog::Severity::verbose, plog::streamStdErr);

    if (!engine_proc.running()) {
        PLOG_ERROR << fmt::format(FMT_COMPILE("Failed to launch executable '{}' at path '{}'."), engine_path, path_to_engine_executable);
        throw new engine_not_launched_exception(fmt::format("Failed to launch '{}' at path '{}'.", engine_path, path_to_engine_executable));
    }
}

EngineWhisperer::~EngineWhisperer() {
    PLOG_DEBUG << fmt::format(FMT_COMPILE("Attempting exit with UCI quit."));
    asio::write(engine_proc, asio::buffer(UCIcommand::quit()));

    asio::steady_timer t(io);
    t.expires_after(std::chrono::seconds(5));

    bool term = false;
    t.async_wait([&](const boost::system::error_code& ec) {
        if(engine_proc.running()) {
            engine_proc.terminate(); 
            term = true;
        } 
    });

    io.run_for(std::chrono::seconds(5));
    PLOG_DEBUG_IF(term) << "Engine did not shut down in 5 seconds, terminated instead.";
    PLOG_DEBUG_IF(!term) << "Engine exited.";

    t.expires_after(std::chrono::seconds(0));

    io.stop();
}


bool EngineWhisperer::start_uci() {
    boost::system::error_code ec;

    if (!engine_proc.running()) {
        throw new engine_not_launched_exception("Engine is not running, can't give command.");
    }

    asio::write(engine_proc, asio::buffer(UCIcommand::uci()));
    
    size_t sz = 0;
    bool set = false;
    bool timeout = false;
    bool finished = false;

    std::string engine_response = read_engine_with_timeout(UCIcommand::EngineCommands::uciok());

    if (engine_response.empty()) {
        return false;
    }
    if (!check_engine_isready()) {
        return false;
    }

    write_engine_with_timeout(UCIcommand::ucinewgame());
    write_engine_with_timeout(UCIcommand::position_startpos());
    
    using sq = chess::Square;
    std::vector<chess::Move> moves{chess::Move::make(sq::SQ_E2, sq::SQ_E4), chess::Move::make(sq::SQ_E7, sq::SQ_E5)};
    make_moves(moves);

    new_game();

    std::vector<chess::Move> fools_mate {
        chess::Move::make(sq::SQ_F2, sq::SQ_F3), 
        chess::Move::make(sq::SQ_E7, sq::SQ_E6),
        
        chess::Move::make(sq::SQ_G2, sq::SQ_G4),
        chess::Move::make(sq::SQ_D8, sq::SQ_H4)
    };

    write_engine_with_timeout(UCIcommand::position_startpos());
    make_moves(fools_mate);

    return true;

}

size_t EngineWhisperer::write_engine_with_timeout(std::string_view command, size_t timeout) {
    if(!engine_proc.running()) {
        PLOG_ERROR << "Engine process is not running.";
        return false;
    }
    if (io.stopped()) {
        io.restart();
    }

    size_t bytes = 0;
    asio::async_write(engine_proc, asio::buffer(command), 
        [&](const boost::system::error_code& ec, size_t bytes_transferred) {
            PLOG_WARNING_IF(ec) << ec.what();
            bytes = bytes_transferred;
        }
    );
    
    size_t handler_count = io.run_for(std::chrono::seconds(timeout));
    PLOG_DEBUG << fmt::format(FMT_COMPILE("Async write to engine complete. Handlers executed: {}"), handler_count);
    
    bool all_sent = bytes == command.length();
    PLOG_WARNING_IF(!all_sent) << fmt::format(FMT_COMPILE("Wrote {} bytes to engine, but command was {} bytes."), bytes, command.length());

    io.stop();

    return bytes;
}

bool EngineWhisperer::check_engine_isready() {
    PLOG_DEBUG << "Asking engine if ready";
    size_t sent = write_engine_with_timeout(UCIcommand::isready(), m_write_timeout);
    if (sent != UCIcommand::isready().size()) {
        PLOG_DEBUG << "Failed to check if engine is ready.";
        return sent;
    }
    std::string ready = read_engine_with_timeout(UCIcommand::EngineCommands::readyok(),m_read_timeout);
    if (ready.rfind(UCIcommand::EngineCommands::readyok()) == std::string::npos) {
        PLOG_DEBUG << fmt::format(FMT_COMPILE("Engine not ready within {} seconds"), m_read_timeout);
        return false;
    }
    PLOG_DEBUG << "Engine ready";
    return true;
}

std::string EngineWhisperer::read_engine_with_timeout(std::string_view search_string, size_t timeout) {
    if(!engine_proc.running()) {
        PLOG_ERROR << "Engine process is not running.";
        return "";
    }
    if (io.stopped()) {
        io.restart();
    }
    std::string buf {};
    size_t sz = 0;
    asio::async_read_until(engine_proc, asio::dynamic_buffer(buf), boost::regex(std::string(search_string)),
    [&](const boost::system::error_code& err, std::size_t bytes_transferred) {
        PLOG_WARNING_IF(err) << err.what();
        }
    );

    size_t handler_count = io.run_for(std::chrono::seconds(timeout));

    PLOG_DEBUG << fmt::format(FMT_COMPILE("Async read from engine complete. Handlers executed: {}"), handler_count);

    io.stop();
    return buf;

}

void EngineWhisperer::new_game() {
    if (!engine_proc.running()) {
        throw new engine_not_launched_exception("Engine not running!");
    }

    PLOG_DEBUG << "Starting new game.";
    write_engine_with_timeout(UCIcommand::ucinewgame());

    check_engine_isready();

    m_board.setFen(chess::constants::STARTPOS);
}

/**
 * @todo Handle possible write errors.
 */
bool EngineWhisperer::set_position(std::string_view fen) {
    if (!engine_proc.running()) {
        throw engine_not_launched_exception("Engine not running!");
    }
    PLOG_DEBUG << fmt::format(FMT_COMPILE("Setting engine position to FEN: {}"), fen);
    write_engine_with_timeout(UCIcommand::create_position_command(fen));
    return true;
}

bool EngineWhisperer::make_moves(chess::Move move) {

    std::vector<chess::Move> m{move};
    return make_moves(m);
}

/**
 * @todo Issue "stop" command on timeout, so evaluation still can be extracted, instead of undoing the moves.
 * @todo Use m_board to validate moves before trying to play them.
 */
bool EngineWhisperer::make_moves(std::vector<chess::Move>& moves) {
    if (!engine_proc.running()) {
        throw new engine_not_launched_exception("Engine not running!");
    }

    std::vector<std::string> mvs;
    mvs.reserve(moves.size());
    
    for (chess::Move m : moves) {
        const chess::PieceGenType piecetype = [&]() {
            chess::PieceType typ = m_board.at<chess::PieceType>(m.from());
            // Ugly switch-case converts types.
            switch (typ.internal()) {
            case chess::PieceType::PAWN:
                return chess::PieceGenType::PAWN;
            case chess::PieceType::underlying::KNIGHT:
                return chess::PieceGenType::KNIGHT;
            case chess::PieceType::underlying::BISHOP:
                return chess::PieceGenType::BISHOP;
            case chess::PieceType::underlying::ROOK:
                return chess::PieceGenType::ROOK;
            case chess::PieceType::underlying::QUEEN:
                return chess::PieceGenType::QUEEN;
            case chess::PieceType::underlying::KING:
                return chess::PieceGenType::KING;
            case chess::PieceType::underlying::NONE:
            default:
                return static_cast<chess::PieceGenType>(63); // This searches for all move types. Case fallthrough to default is intentional.
                // ideally we don't hit this, because this will generate a lot more unnessecary moves.
            }
        }();

        chess::Movelist m_gen;
        chess::movegen::legalmoves<chess::movegen::MoveGenType::ALL>(m_gen, m_board, piecetype);

        const std::string move_as_uci = chess::uci::moveToUci(m);

        auto it = std::find(m_gen.begin(), m_gen.end(), m);
        PLOG_DEBUG_IF(it != m_gen.end()) << fmt::format(FMT_COMPILE("Move {} is legal in current position {}"), move_as_uci, m_board.getFen());
        PLOG_WARNING_IF(it == m_gen.end()) << fmt::format(FMT_COMPILE("Move {} is not legal in current position {}. This error is NOT handled."), move_as_uci, m_board.getFen());

        m_board.makeMove(m);
        mvs.push_back(move_as_uci);
    }

    std::string cmd = UCIcommand::append_moves_to_position_command(UCIcommand::create_position_command(m_board.getFen()), moves);
    std::string_view cmd_view(cmd);

    PLOG_DEBUG << fmt::format(FMT_COMPILE("Playing moves {}"), fmt::join(mvs, " "));
    size_t bytes = write_engine_with_timeout(cmd_view);
    //size_t bytes = asio::write(engine_proc, asio::buffer(cmd_view));
    if (bytes != cmd.length()) {
        PLOG_WARNING << fmt::format(FMT_COMPILE("Wrote {} bytes to engine, but command was {} bytes. Undoing moves."), bytes, cmd.length());
        for (auto it = moves.rbegin(); it != moves.rend(); it++) {
            m_board.unmakeMove(*it);
        }
        return false;
    }
    current_position_fen = m_board.getFen();

    bytes = write_engine_with_timeout(UCIcommand::create_go_depth_command(m_depth));
    std::string read_buf = read_engine_with_timeout("bestmove");

    if (read_buf.empty()) {
        PLOG_WARNING << "No best move given by engine";
        return false;
    }

    bool is_mate = false;

    std::stringstream rs(read_buf);
    std::vector<std::string> lines;
    std::string ln {};

    while (std::getline(rs, ln)) {
        lines.push_back(ln);
    }

    auto last_info_str = lines.end()-2;

    boost::smatch eval_match;
    boost::regex eval_regex("(?<centipawns>(cp\\s*(?<eval>\\d+)))|(?<mate>(mate\\s*(?<count>\\d+)))");
    PLOG_DEBUG << fmt::format(FMT_COMPILE("Getting moves with regex: {}"), eval_regex.str());
    PLOG_DEBUG << fmt::format(FMT_COMPILE("Subject string: {}"), *last_info_str);
    bool eval_matched = boost::regex_search(std::string(*last_info_str), eval_match, eval_regex, boost::match_extra);

    PLOG_DEBUG_IF(eval_match["eval"].matched) << fmt::format(FMT_COMPILE("Sub-expression \"{}\" matched with string \"{}\""), "eval", eval_match["eval"].str());
    PLOG_DEBUG_IF(eval_match["count"].matched) << fmt::format(FMT_COMPILE("Sub-expression \"{}\" matched with string \"{}\""), "count", eval_match["count"].str());
    PLOG_WARNING_IF(!eval_matched) << fmt::format(FMT_COMPILE("No match for regex."));
    
    if (eval_match["eval"].matched) {
        m_eval.setEval(static_cast<double>(std::stoi(eval_match["eval"].str()))/100.0f);
    }

    std::pair<chess::GameResultReason, chess::GameResult>is_over = m_board.isGameOver();

    if (is_over.second /* if the game is over */ != chess::GameResult::NONE) {
        
        switch (is_over.first /* Reason game is over */) {

        case chess::GameResultReason::CHECKMATE:
            PLOGD << fmt::format(FMT_COMPILE("Game is over by checkmate!"));
            m_eval.setMatePlayed(true);
            break;
        case chess::GameResultReason::STALEMATE:
            PLOGD << fmt::format(FMT_COMPILE("Game is drawn by stalemate!"));
            break;
        case chess::GameResultReason::INSUFFICIENT_MATERIAL:
            PLOGD << fmt::format(FMT_COMPILE("Game is drawn by insufficient material!"));
            break;
        case chess::GameResultReason::FIFTY_MOVE_RULE:
            PLOGD << fmt::format(FMT_COMPILE("Game is drawn by the 50 move rule!"));
            break;
        case chess::GameResultReason::THREEFOLD_REPETITION:
            PLOGD << fmt::format(FMT_COMPILE("Game is drawn by threefold repetition!"));
            break;
        case chess::GameResultReason::NONE:
        default:
          break;
        }

        return true;
    }

    size_t moves_to_mate = 0;
    if (eval_match["count"].matched) {
        is_mate = true;
        PLOGD << eval_match["count"].str();
        moves_to_mate = std::stoi(eval_match["count"].str());
    }

    boost::regex rgx("bestmove\\s(?<bestmove>([a-h]\\d){2})\\s*(ponder\\s(?<ponder>([a-h]\\d){2}\\s*))?");
    boost::smatch match;
    
    std::string& bestmove_str = lines.back();
    PLOG_DEBUG << fmt::format(FMT_COMPILE("Getting moves with regex: {}"), rgx.str());
    PLOG_DEBUG << fmt::format(FMT_COMPILE("Subject string: {}"), bestmove_str);
    
    bool test = boost::regex_search(bestmove_str, match, rgx, boost::match_extra);
    
    PLOG_DEBUG_IF(test) << fmt::format(FMT_COMPILE("Regex matched!"));
    PLOG_WARNING_IF(!test) << fmt::format(FMT_COMPILE("No regex matches AND no error handling for this!"));
    PLOG_DEBUG_IF(match["bestmove"].matched) << fmt::format(FMT_COMPILE("Sub-expression \"{}\" matched with string \"{}\""), "bestmove", match["bestmove"].str());
    PLOG_DEBUG_IF(match["ponder"].matched) << fmt::format(FMT_COMPILE("Sub-expression \"{}\" matched with string \"{}\""), "ponder", match["ponder"].str());

    chess::Square from(match["bestmove"].str().substr(0, 2));
    chess::Square to(match["bestmove"].str().substr(2,2));

    chess::Move best = chess::Move::make(from, to);

    PLOG_DEBUG << fmt::format(FMT_COMPILE("Got best move from regex: {}"), chess::uci::moveToUci(best));

    if (!is_mate) {
        chess::Square p_from(match["ponder"].str().substr(0, 2));
        chess::Square p_to(match["ponder"].str().substr(2, 2));
        chess::Move p = chess::Move::make(p_from, p_to);
        PLOG_DEBUG << fmt::format(FMT_COMPILE("Got ponder from regex: {}"), chess::uci::moveToUci(p));
        m_eval.setPonder(p);
    }
    m_eval.setBestmove(best);
    m_eval.update(m_board.sideToMove(), m_eval.getEval(), best, is_mate ? chess::Move(0) : m_eval.getPonder(), is_mate, m_board.sideToMove(), moves_to_mate);
    PLOG_DEBUG << fmt::format("{}", m_eval.to_string());

    return true;
}

std::pair<size_t, std::string> EngineWhisperer::write_and_read_with_timeout(std::string_view command, std::string_view search_string, size_t timeout) {
    size_t sz = write_engine_with_timeout(command, timeout);
    if (sz != command.length()) {
        return {sz, ""};
    }
    std::string read = read_engine_with_timeout(search_string, timeout);
    return {sz, read};
}

/**
 * @todo Return an error state in Evaluation struct on empty return string or wrong amount of bytes written.
 */
std::optional<Evaluation> EngineWhisperer::naive_eval_from_position(std::string_view fen) {

    std::optional<Evaluation> eval_out;
    new_game();

    std::string cmd = UCIcommand::create_position_command(fen);

    std::pair<size_t, std::string> result = write_and_read_with_timeout(
        cmd,
        UCIcommand::EngineCommands::bestmove()
    );
    if (result.first != cmd.length()) {
        return eval_out;
    }
    if (result.second.empty()) {
        return eval_out;
    }

    std::vector<std::string> lines;
    std::stringstream ss(result.second);
    std::string line{};
    while (std::getline(ss, line)) {
        lines.push_back(line);
    }

    std::string& bestmove_str = lines.back();
    auto last_info_str = lines.end()-2;
    auto bestmoveponder = extractBestmoveFromRegex(bestmove_str);

    if (!bestmoveponder) {
        return eval_out;
    }

    

    if (std::holds_alternative<TwoMoves>(bestmoveponder.value())) {
        eval_out.emplace();
        TwoMoves best_ponder = std::get<TwoMoves>(bestmoveponder.value());
        eval_out->setBestmove(best_ponder.first);
        eval_out->setPonder(best_ponder.second);
    } else if (std::holds_alternative<chess::Move>(bestmoveponder.value())) {
        eval_out.emplace();
        chess::Move best = std::get<chess::Move>(bestmoveponder.value());
        eval_out->setBestmove(best);
    }

    // switch (bestmoveponder->index()) {
    //     case 0: { /* Variant is type std::pair<chess::Move, chess::Move> */
    //         eval_out.emplace();
    //         auto best_ponder = std::get<std::pair<chess::Move, chess::Move>>(bestmoveponder.value());
    //         eval_out->setBestmove(best_ponder.first);
    //         eval_out->setPonder(best_ponder.second);
    //     }
    //     case 1: { /* Variant is type chess::Move */
    //         eval_out.emplace();
    //         auto best = std::get<chess::Move>(bestmoveponder.value());
    //         eval_out->setBestmove(best);
    //     }
    //     default:
    //         break;
    // }
    
    auto eval = extractEvalFromRegex(std::string_view(*last_info_str));

    if (!eval) {
        return eval_out;
    }

    if (std::holds_alternative<double>(eval.value())) {
        if (!eval_out) {
            eval_out.emplace();
        }
        double centipawns = std::get<double>(eval.value());
        eval_out->setEval(centipawns);
    } else if (std::holds_alternative<size_t>(eval.value())) {
        if (!eval_out) {
            eval_out.emplace();
        }
        size_t moves_to_mate = std::get<size_t>(eval.value());
        eval_out->setMateCount(moves_to_mate);
        eval_out->setMate(true);
    }

    return eval_out;

}

std::optional<std::variant<std::pair<chess::Move, chess::Move>, chess::Move>> EngineWhisperer::extractBestmoveFromRegex(std::string_view input) {

    const std::string bestmove_capture = "bestmove";
    const std::string ponder_capture = "ponder";
    boost::regex rgx("bestmove\\s(?<bestmove>([a-h]\\d){2})\\s*(ponder\\s(?<ponder>([a-h]\\d){2}\\s*))?");

    PLOG_DEBUG << fmt::format(FMT_COMPILE("Getting moves with regex: {}"), rgx.str());
    PLOG_DEBUG << fmt::format(FMT_COMPILE("Subject string: {}"), input);

    boost::smatch matches;
    bool matched_all = boost::regex_search(std::string(input), matches, rgx, boost::match_extra);

    bool bestmove_matched = matches[bestmove_capture].matched;
    bool ponder_matched   = matches[ponder_capture].matched;

    bool both_matched = ponder_matched && bestmove_matched;
    if (!both_matched) {
        return {};
    }
    if (bestmove_matched) {
        chess::Square from(matches[bestmove_capture].str().substr(0, 2));
        chess::Square to(matches[bestmove_capture].str().substr(2,2));
        chess::Move best = chess::Move::make(from, to);

        if (ponder_matched) {
            chess::Square p_from(matches["ponder"].str().substr(0, 2));
            chess::Square p_to(matches["ponder"].str().substr(2, 2));
            chess::Move p = chess::Move::make(p_from, p_to);
            return std::pair<chess::Move, chess::Move>(best, p);
        }
        return best;
    } 
    return {};
}

std::optional<std::variant<double, size_t>> EngineWhisperer::extractEvalFromRegex(std::string_view input) {
    const std::string eval_cap = "eval";
    const std::string mate_cap = "count";

    boost::regex eval_rgx("(?<centipawns>(cp\\s*(?<eval>\\d+)))|(?<mate>(mate\\s*(?<count>\\d+)))");
    boost::smatch matches;

    bool match = boost::regex_search(std::string(input), matches, eval_rgx, boost::match_extra);
    bool eval_matched = matches[eval_cap].matched;
    bool mate_matched = matches[mate_cap].matched;

    if (eval_matched & mate_matched) {
        return {};
    }
    if (eval_matched) {
        return static_cast<double>(std::stoi(matches[eval_cap].str())/100.0f);
    }
    if (mate_matched) {
        return std::stoul(matches[mate_cap].str());
    }
    return {};
}

double EngineWhisperer::getPositionEval() {
    return m_eval.getEval();
}
