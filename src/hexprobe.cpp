#include <iostream>
#include <vector>
#include "hexprobe.h"

using Byte = uint8_t;
using Buffer = std::vector<Byte>;

int start(Input input) {
    std::cout << "输入的文件名是：" << input.file_name << "\n";
    std::cout << "输入的起始偏移为：" << input.start_offset << "\n";
    
    if(!input.end_offset) {
        std::cout << "未输入终止偏移。\n";
    } else {
        std::cout << "输入的终止位移为：" << *input.end_offset << "\n";
    }

    if(!input.length) {
        std::cout << "未输入区间长度。\n";
    } else {
        std::cout << "输入的区间长度为：" << *input.length << "\n";
    }

    return 0;
}
