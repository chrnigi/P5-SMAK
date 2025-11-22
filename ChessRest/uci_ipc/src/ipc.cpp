#include <chess.hpp>
#include <ipc.hpp>
#include <uci_commands.hpp>

#include <boost/asio.hpp>
#include <boost/process/v2.hpp>
#include <boost/regex.hpp>

#include <fmt/core.h>
#include <fmt/compile.h>
#include <fmt/format.h>

#include <string_view>
#include <string>
#include <chrono>



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
        fmt::println("Still running...");
    }

    t.expires_after(std::chrono::seconds(0));

}


bool EngineWhisperer::start_uci() {
    boost::system::error_code ec;

    if (!engine_proc.running()) {
        throw new engine_not_launched_exception("Engine is not running, can't give command.");
    }

    asio::write(engine_proc, "\n uci\n"_buf);

    std::string buffer {};
    asio::steady_timer timer(io);
    
    size_t sz = 0;
    bool set = false;
    bool timeout = false;
    bool finished = false;

    asio::async_read_until(engine_proc, asio::dynamic_buffer(buffer), boost::regex("uciok"), 
        [&](const boost::system::error_code& err, std::size_t bytes_transferred) {
            fmt::println("{}", ec.what());
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
    asio::write(engine_proc, asio::buffer(UCIcommand::ucinewgame()));
    asio::write(engine_proc, asio::buffer(UCIcommand::position_startpos()));
    return true;

}

void EngineWhisperer::new_game() {
    if (!engine_proc.running()) {
        throw new engine_not_launched_exception("Engine not running!");
    }

    asio::write(engine_proc, asio::buffer(UCIcommand::ucinewgame()));
}

bool EngineWhisperer::set_position(std::string_view fen) {
    if (!engine_proc.running()) {
        throw engine_not_launched_exception("Engine not running!");
    }

    asio::write(engine_proc, asio::buffer(UCIcommand::create_position_command(fen)));
    return true;
}

bool EngineWhisperer::make_move(int16_t to, int16_t from, bool promotion) {
    
    const chess::Move mv = [&](){
        if (promotion) {
            return chess::Move::make<chess::Move::PROMOTION>(to, from, chess::PieceType::QUEEN);
        } 
        return chess::Move::make(to, from);
    }();

    if (!engine_proc.running()) {
        throw new engine_not_launched_exception("Engine not running!");
    }
    std::string cmd = UCIcommand::create_position_command(current_position_fen);
    cmd = UCIcommand::append_moves_to_position_command(cmd, mv);
    asio::write(engine_proc, asio::buffer(cmd));

    return true;
}
