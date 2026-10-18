#include <iostream>
#include <optional>
#include <ctype.h>
#include "display.h"

void print_newline(void);
void print_hex(uint32_t n);
void print_byte(uint8_t byte);
void print_wide_blank(std::vector<std::optional<uint8_t>> line);
void print_blank(std::vector<std::optional<uint8_t>> line);

void print_header(uint32_t start_offset, uint32_t length) {
    uint32_t end_offset = start_offset + length - 1;

    print_newline();
    std::cout << "从 0x";
    print_hex(start_offset);
    std::cout << " 开始，读取 " << length << " 个字节。(0x";
    print_hex(start_offset);

    if(length > 1) {
        std::cout << " - 0x";
        print_hex(end_offset);
    }
    std::cout << ")\n\n";
}

void print_newline(void) {
    std::cout << "\n";
}

void print_hex(uint32_t n) {
    constexpr char digits[] = "0123456789ABCDEF";
    char hex[8];
    for (int i = 7; i >= 0; --i) {
        hex[i] = digits[n & 0xF];
        n >>= 4;
    }
    std::cout.write(hex, sizeof(hex));
}

void print_byte(uint8_t byte) {
    constexpr char digits[] = "0123456789ABCDEF";
    const char hex[2] = {digits[byte >> 4], digits[byte & 0x0F]};
    std::cout.write(hex, sizeof(hex));
}

void print_table(std::vector<uint8_t> &buffer, uint32_t start_offset, uint32_t length) {
    std::cout << "           00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n";

    uint32_t pointer = (start_offset / 16) * 16;
    while(pointer < start_offset+length) {
        std::cout << "0x";
        print_hex(pointer);
        std::cout << " ";

        std::vector<std::optional<uint8_t>> line;
        for(int i = 0; i < 16; i++) {
            uint32_t pointer_offset = pointer + i;
            if(pointer_offset < start_offset || pointer_offset >= start_offset+length) {
                line.push_back(std::nullopt);
            } else {
                line.push_back(buffer[pointer_offset-start_offset]);
            }
        }

        print_wide_blank(line);
        print_blank(line);
        print_newline();
        pointer += 16;
    }
}

void print_wide_blank(std::vector<std::optional<uint8_t>> line) {
    for(std::optional<uint8_t> byte : line) {
        if(byte) {
            print_byte(*byte);
            std::cout << " ";
        } else {
            std::cout << "   ";
        }
    }
}

void print_blank(std::vector<std::optional<uint8_t>> line) {
    for(std::optional<uint8_t> byte : line) {
        if(byte) {
            if(!isprint(*byte)) {
                std::cout << ".";
            } else {
                std::cout << *byte;
            }
        } else {
            std::cout << " ";
        }
    }
}
