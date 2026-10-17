#pragma once
#include <vector>
#include <string>

using Messages = std::vector<std::string>;

namespace StatusCode {
constexpr int OK = 0;
constexpr int UnexpectedArgument = 1;
constexpr int UnexpectedBreak = 2;
constexpr int StartOffsetRepeat = 3;
constexpr int StartOffsetInvalidNumber = 4;
constexpr int EndOffsetRepeat = 5;
constexpr int EndOffsetInvalidNumber = 6;
constexpr int ArgumentConfilt = 7;
constexpr int LengthRepeat = 8;
constexpr int LengthInvalidNumber = 9;
}

void print_errmsg(int status_code);