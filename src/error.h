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
constexpr int FileIsNotExist = 10;
constexpr int FileIsEmpty = 11;
constexpr int RangeIsNotExist = 12;
constexpr int OverRange = 13;
constexpr int RangeIsTooLong = 14;
constexpr int ReadFileError = 15;
}

void print_errmsg(int status_code);