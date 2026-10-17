#include <iostream>
#include "error.h"

void print_errmsg(int status_code) {
    Messages errmsg = {
        "",
        "",
        "意料外的终止。",
        "--start / -s 参数重复出现。",
        "在应该填入起始偏移参数的位置，填入了非数字字符串。",
        "--end / -e 参数重复出现。",
        "在应该填入终止偏移参数的位置，填入了非数字字符串。",
        "--end / -e 参数和 --length / -l 参数同时出现。",
        "--length / -l 参数重复出现。",
        "在应该填入终止区间的位置，填入了非数字字符串。"
    };
    std::cout << "错误：" << errmsg[status_code] << "\n";
}
