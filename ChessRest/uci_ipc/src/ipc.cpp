#include <chess.hpp>
#include <ipc.hpp>
#include <uci_commands.hpp>

#include <boost/asio.hpp>
#include <boost/process/v2.hpp>
#include <boost/regex.hpp>

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
#include <vector>

namespace asio = boost::asio;
namespace procv2 = boost::process::v2;
using namespace asio::buffer_literals;


EngineWhisperer::EngineWhisperer(std::string engine_path) :
    path_to_engine_executable(procv2::environment::find_executable(engine_path).string()),
    engine_proc(io, path_to_engine_executable, {})
{
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
        
        chess::Move::make(sq::SQ_G2, sq::SQ_G4)
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
 */
bool EngineWhisperer::make_moves(std::vector<chess::Move>& moves) {
    if (!engine_proc.running()) {
        throw new engine_not_launched_exception("Engine not running!");
    }

    std::string cmd = UCIcommand::append_moves_to_position_command(UCIcommand::create_position_command(m_board.getFen()), moves);
    std::vector<std::string> mvs;
    mvs.reserve(moves.size());
    for (chess::Move m : moves) {
        m_board.makeMove(m);
        mvs.push_back(chess::uci::moveToUci(m));
    }

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
    size_t moves_to_mate = 0;
    if (eval_match["count"].matched) {
        is_mate = true;
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

    std::string sq_from(match["bestmove"], 0, 2);
    std::string sq_to(match["bestmove"],2,2);
    chess::Square from(match["bestmove"].str().substr(0, 2));
    chess::Square to(sq_to);

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

double EngineWhisperer::getPositionEval() {
    return m_eval.getEval();
}
