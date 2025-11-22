#include "uci_commands.hpp"
#include <boost/asio/buffer.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/write.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/system/detail/error_code.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/process/v2/environment.hpp>
#include <boost/process/v2/popen.hpp>
#include <chrono>
#include <cstddef>
#include <ipc.hpp>
#include <boost/asio.hpp>
#include <boost/process/v2.hpp>
#include <boost/regex.hpp>
#include <boost/chrono.hpp>
#include <fmt/core.h>

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

    // timer.expires_after(std::chrono::seconds(5)); // Wait up to 5 sec to get "uciok"
    
    // timer.async_wait([&](const boost::system::error_code ec){
    //     fmt::println("Timed out!");
    //     set = true;
    //     timeout = true;
    // });



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
    io.run_for(std::chrono::seconds(5));
    fmt::println("{}", set);
    while (!set) {} 

    if (timeout) {
        return false;
    }
    
    asio::write(engine_proc, "\n position startpos\n"_buf);
    return true;

}


EngineWhisperer::~EngineWhisperer() {
    
}
