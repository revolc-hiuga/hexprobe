#include <iostream>
#include <cstdint>
#include <vector>
#include "version.h"
#include "hexprobe.h"
#include "error.h"

#ifdef _WIN32
#include <windows.h>
#endif

using Arguments = std::vector<std::string>;

struct ParseResult {
    ParseResult(int _status_code, std::optional<Input> _input_opt):
        status_code(_status_code), input_opt(_input_opt) {}

    int status_code;
    std::optional<Input> input_opt;
};

void init_codepage();
void help();
Arguments collect_arguments(int argc, char** argv);
ParseResult parse_arguments(Arguments args);
std::optional<uint32_t> convert_hex(std::string s);
std::optional<uint32_t> convert_dec(std::string s);

int main(int argc, char** argv) {
    init_codepage();

    Arguments args = collect_arguments(argc, argv);
    ParseResult parse_result = parse_arguments(args);
    if(parse_result.status_code != StatusCode::OK) {
        if(parse_result.status_code == StatusCode::UnexpectedArgument) {
            help();
        } else {
            print_errmsg(parse_result.status_code);
        }
        return 1;
    }
    return start(*parse_result.input_opt);
}

void init_codepage() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif
}

void help() {
    std::cout << "HexProbe 十六进制浏览器\n"
              << "作者：RevoLC build: " << BuildNumber << "\n\n"

              << "hexprobe 文件名 <--start/-s 起始偏移> <--end/-e 终止偏移>\n"
              << "hexprobe 文件名 <--start/-s 起始偏移> <--length/-of 区间长度>\n\n"

              << "【参数含义】\n"
              << "--start/-s\t\t程序从起始偏移开始读取数据。起始偏移为十六进制数，接受 ""0x"" 前缀。\n"
              << "--end/-e\t\t程序读取数据到终止偏移为止停止。终止位移为十六进制数，接受 ""0x"" 前缀。\n"
              << "\t\t\t不能与 --length/-l 参数同时使用。\n"
              << "--length/-l\t\t程序读取给定长度的数据。区间长度为十进制数。\n"
              << "\t\t\t不能与 --end/-e 参数同时使用。\n\n"
              
              << "【注意事项】\n"
              << "除了文件名，所有参数都是可选的。\n"
              << "起始偏移默认为 0, 区间长度默认为 256. 如果起始偏移距离文件末尾的距离小于 256, 取这段距离。\n"
              << "区间长度不能超过 1024.\n";
}

Arguments collect_arguments(int argc, char** argv) {
    Arguments args;
    for(int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }
    return args;
}

ParseResult parse_arguments(Arguments args) {
    if(args.size() == 1) {
        return ParseResult(StatusCode::UnexpectedArgument, std::nullopt);
    } else if(args.size() == 2) {
        return ParseResult(StatusCode::OK,
                           Input(args[1], 0, std::nullopt, 256));
    }
    
    uint32_t start_offset = 0;
    std::optional<uint32_t> end_offset, length;

    bool is_start_offset_set = false;
    bool is_end_offset_set = false;
    bool is_length_set = false;

    for(size_t i = 2; i < args.size(); i++) {
        std::string token = args[i];

        if(token == "--start" || token == "-s") {
            if(is_start_offset_set) {
                return ParseResult(StatusCode::StartOffsetRepeat, std::nullopt);
            }

            if(i == args.size()-1) {
                return ParseResult(StatusCode::UnexpectedBreak, std::nullopt);
            }

            std::optional<int> start_offset_opt = convert_hex(args[i+1]);
            if(!start_offset_opt) {
                return ParseResult(StatusCode::StartOffsetInvalidNumber, std::nullopt);
            }
            
            start_offset = *start_offset_opt;
            is_start_offset_set = true;
            i++;
        } else if(token == "--end" || token == "-s") {
            if(is_end_offset_set) {
                return ParseResult(StatusCode::EndOffsetRepeat, std::nullopt);
            }

            if(i == args.size()-1) {
                return ParseResult(StatusCode::UnexpectedBreak, std::nullopt);
            }

            end_offset = convert_hex(args[i+1]);
            if(!end_offset) {
                return ParseResult(StatusCode::EndOffsetInvalidNumber, std::nullopt);
            }

            is_end_offset_set = true;
            i++;
        } else if(token == "--length" || token == "-l") {
            if(is_end_offset_set) {
                return ParseResult(StatusCode::ArgumentConfilt, std::nullopt);
            }

            if(is_length_set) {
                return ParseResult(StatusCode::LengthRepeat, std::nullopt);
            }

            if(i == args.size()-1) {
                return ParseResult(StatusCode::UnexpectedArgument, std::nullopt);
            }

            length = convert_dec(args[i+1]);
            if(!length) {
                return ParseResult(StatusCode::LengthInvalidNumber, std::nullopt);
            }

            is_length_set = true;
            i++;
        } else {
            return ParseResult(StatusCode::UnexpectedArgument, std::nullopt);
        }
    }

    return ParseResult(StatusCode::OK, Input(args[1], start_offset, end_offset, length));
}

std::optional<uint32_t> convert_hex(std::string s) {
    if(s.compare(0, 2, "0x") == 0) {
        s.erase(0, 2);
    }
    if(s.empty()) {
        return std::nullopt;
    }

    uint32_t value = 0;
    for(char c : s) {
        uint32_t digit;
        if(c >= '0' && c <= '9') {
            digit = c - '0';
        } else if(c >= 'a' && c <= 'f') {
            digit = c - 'a' + 10;
        } else if(c >= 'A' && c <= 'F') {
            digit = c - 'A' + 10;
        } else {
            return std::nullopt;
        }

        if(value > (UINT32_MAX - digit) / 16) {
            return std::nullopt;
        }
        value = value * 16 + digit;
    }
    return value;
}

std::optional<uint32_t> convert_dec(std::string s) {
    if(s.empty() || (s.size() > 1 && s[0] == '0')) {
        return std::nullopt;
    }

    uint32_t value = 0;
    for(char c : s) {
        if(c < '0' || c > '9') {
            return std::nullopt;
        }

        uint32_t digit = c - '0';
        if(value > (UINT32_MAX - digit) / 10) {
            return std::nullopt;
        }
        value = value * 10 + digit;
    }
    return value;
}
