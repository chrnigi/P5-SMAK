#pragma once

#include <string_view>
#ifndef UCI_COMMANDS_HPP
#define UCI_COMMANDS_HPP


struct UCIcommand {
    static constexpr std::string_view uci = "uci";
    static constexpr std::string_view position = "position";
};

#endif
