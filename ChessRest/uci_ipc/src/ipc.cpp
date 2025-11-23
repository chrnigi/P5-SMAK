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
    if (!engine_proc.running()) {
        throw new engine_not_launched_exception(fmt::format("Failed to launch '{}' at path '{}'.", engine_path, path_to_engine_executable));
    }
}

EngineWhisperer::~EngineWhisperer() {
    asio::write(engine_proc, asio::buffer(UCIcommand::quit()));

    asio::steady_timer t(io);
    t.expires_after(std::chrono::seconds(5));

    bool term = false;
    t.async_wait([&](const boost::system::error_code& ec) {engine_proc.terminate(); fmt::println("Engine process terminated!"); term = true;});


    io.run_for(std::chrono::seconds(5));

    while (engine_proc.running()) {
        //fmt::println("Still running...");
    }

    t.expires_after(std::chrono::seconds(0));

    io.stop();
}


bool EngineWhisperer::start_uci() {
    boost::system::error_code ec;

    if (!engine_proc.running()) {
        throw new engine_not_launched_exception("Engine is not running, can't give command.");
    }

    asio::write(engine_proc, asio::buffer(UCIcommand::uci()));

    std::string buffer {};
    asio::steady_timer timer(io);
    
    size_t sz = 0;
    bool set = false;
    bool timeout = false;
    bool finished = false;

    if (io.stopped())
        io.restart();

    asio::async_read_until(engine_proc, asio::dynamic_buffer(buffer), boost::regex("uciok"), 
        [&](const boost::system::error_code& err, std::size_t bytes_transferred) {
            fmt::println("{}", err.what());
            if (!set) {
                set = true;
            }
            if (timeout) {
                return;
            } else {
                sz = bytes_transferred;
                finished = true;
            }

        }
    );
    size_t c = io.run_for(std::chrono::seconds(5));
    fmt::println("{}", fmt::format(FMT_COMPILE("{}, handlers: {}"), set, c)) ;
    while (!set) {} 

    if (timeout) {
        return false;
    }

    buffer.erase();
    asio::write(engine_proc, asio::buffer("\n isready\n"));
    asio::read_until(engine_proc, asio::dynamic_buffer(buffer), boost::regex("readyok"));
    fmt::println("{}", buffer);
    asio::write(engine_proc, asio::buffer(UCIcommand::ucinewgame()));
    asio::write(engine_proc, asio::buffer(UCIcommand::position_startpos()));
    

    using sq = chess::Square;
    std::vector<chess::Move> moves{chess::Move::make(sq::SQ_E2, sq::SQ_E4), chess::Move::make(sq::SQ_E7, sq::SQ_E5)};
    make_moves(moves);

    

    return true;

}

void EngineWhisperer::new_game() {
    if (!engine_proc.running()) {
        throw new engine_not_launched_exception("Engine not running!");
    }

    asio::write(engine_proc, asio::buffer(UCIcommand::ucinewgame()));
    m_board.setFen(chess::constants::STARTPOS);
}

bool EngineWhisperer::set_position(std::string_view fen) {
    if (!engine_proc.running()) {
        throw engine_not_launched_exception("Engine not running!");
    }

    asio::write(engine_proc, asio::buffer(UCIcommand::create_position_command(fen)));
    return true;
}

bool EngineWhisperer::make_moves(chess::Move move) {

    std::vector<chess::Move> m{move};
    return make_moves(m);
}

bool EngineWhisperer::make_moves(std::vector<chess::Move>& moves) {
    if (!engine_proc.running()) {
        throw new engine_not_launched_exception("Engine not running!");
    }

    std::string cmd = UCIcommand::append_moves_to_position_command(UCIcommand::create_position_command(m_board.getFen()), moves);
    
    for (chess::Move m : moves) {
        m_board.makeMove(m);
    }

    std::string_view cmd_view(cmd);
    size_t bytes = asio::write(engine_proc, asio::buffer(cmd_view));
    if (bytes != cmd.length()) {
        for (auto it = moves.rbegin(); it != moves.rend(); it++) {
            m_board.unmakeMove(*it);
        }
        return false;
    }
    current_position_fen = m_board.getFen();

    size_t sz = 0;
    bool set = false;
    bool timeout = false;
    bool finished = false;

    asio::write(engine_proc, asio::buffer(UCIcommand::create_go_depth_command(m_depth)));

    if (io.stopped()) {
        io.restart();
    }

    std::string read_buf {};
    asio::async_read_until(engine_proc, asio::dynamic_buffer(read_buf), boost::regex("bestmove"),
        [&](const boost::system::error_code& err, std::size_t bytes_transferred) {
            fmt::println("{}", fmt::format(FMT_COMPILE("{}"), err.what()));
            if (timeout) {
                return;
            }
            if (!set) {
                set = true;
            }
            sz = bytes_transferred;
            finished = true;
        }
    );

    std::size_t handler_count = io.run_for(std::chrono::seconds(5));
    fmt::println("{}", fmt::format(FMT_COMPILE("{}, handlers: {}"), set, handler_count));
    while (!set) {}

    io.stop();
    if (timeout) {
        return false;
    }

    fmt::println("{}", read_buf);

    // Extract the evaluation/moves to mate from the engine's output.
    std::string last_info = read_buf.substr(read_buf.rfind("info"));
    size_t first = last_info.find("score cp");
    if (first == std::string::npos) {
        first = last_info.find("score mate");
        if (first == std::string::npos) {
            return false;
        }
    }
    size_t last = last_info.find("nodes");
    
    std::string_view extracted(last_info.c_str()+first, last-first);
    fmt::println("{}, {} to move", extracted, m_board.sideToMove() ? "black" : "white");

    return true;
}

Evaluation EngineWhisperer::getPositionEval() {

}
