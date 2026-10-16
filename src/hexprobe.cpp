#include <iostream>
#include <vector>
#include "hexprobe.h"

using Byte = uint8_t;
using Buffer = std::vector<Byte>;

int start(Input input) {
    std::cout << "输入的文件名是：" << input.file_name << "\n";
    std::cout << "输入的起始偏移为：" << input.start_offset << "\n";
    return 0;
}
