#include <fmt/core.h>
#include <ipc.hpp>

int main() {
    
    EngineWhisperer ew("stockfish");
    ew.start_uci();
    return 0;

}
