#include <iostream>
#include <cstdint>
#include <vector>
#include "version.h"
#include "hexprobe.h"

#ifdef _WIN32
#include <windows.h>
#endif

using Arguments = std::vector<std::string>;
using Byte = uint8_t;
using Buffer = std::vector<Byte>;

namespace ParseStatus {
constexpr int OK = 0;
constexpr int UnexpectedArgument = 1;
}

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

int main(int argc, char** argv) {
    init_codepage();

    Arguments args = collect_arguments(argc, argv);
    ParseResult parse_result = parse_arguments(args);
    if(parse_result.status_code != ParseStatus::OK) {
        if(parse_result.status_code == ParseStatus::UnexpectedArgument) {
            help();
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
              << "起始偏移默认为 0, 区间长度默认为 256.\n"
              << "起始偏移和终止偏移（包括指定区间长度模式下的实际终止偏移）不能超过 0x40000000 （即 1GB 大小。）\n"
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
        return ParseResult(ParseStatus::UnexpectedArgument, std::nullopt);
    } else if(args.size() == 2) {
        return ParseResult(ParseStatus::OK,
                           Input(args[1], 0, std::nullopt, 256));
    }
    return ParseResult(ParseStatus::UnexpectedArgument, std::nullopt);
}
