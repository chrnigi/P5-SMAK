#include <boost/asio/any_completion_executor.hpp>
#include <boost/asio/async_result.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/completion_condition.hpp>
#include <boost/asio/writable_pipe.hpp>
#include <boost/process/v2/popen.hpp>
#include <fmt/core.h>
#include <boost/regex.hpp>
#include <boost/process/v2.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/system/detail/error_code.hpp>
#include <iostream>
#include <istream>
#include <ostream>

namespace asio = boost::asio;
namespace procv2 = boost::process::v2;
using namespace asio::buffer_literals;

int main() {
    asio::io_context io;
    asio::readable_pipe p{io};
    asio::writable_pipe wp{io};

    const auto stockfish_exec = procv2::environment::find_executable("stockfish");

    // procv2::process c{io, stockfish_exec, {}, procv2::process_stdio{stdin, stdout, {}}};

    

    // if (!(wp.is_open())) {
    //     return -1;
    // }
    

    // asio::write(wp, "uci\n"_buf );
    boost::process::v2::popen proc(io, stockfish_exec, {});

    std::string line;

    boost::system::error_code ec;
    asio::write(proc, "uci\n"_buf);
    asio::write(proc, "position fen rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1\n"_buf, ec);
    asio::write(proc, asio::buffer("\n go depth 20\n"), ec);
    asio::read_until(proc, asio::dynamic_buffer(line), boost::regex("bestmove"));
    // size_t sz = asio::read(p, asio::dynamic_buffer(line), ec);
    fmt::println("{}", line);

    line.erase();

    asio::write(proc, "\n position fen rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1 moves c7c5\n"_buf);
    asio::write(proc, "\n go depth 5\n"_buf);
    asio::read_until(proc, asio::dynamic_buffer(line), boost::regex("bestmove"));
    fmt::println("{}", line);

    asio::write(proc, "\n quit\n"_buf);

    proc.wait();
    return 0;


}
