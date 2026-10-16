#pragma once

#include <cstdint>
#include <string>
#include <optional>

struct Input {
    Input(std::string _file_name, uint32_t _start_offset, 
          std::optional<uint32_t> _end_offset, std::optional<uint32_t> _length):
        file_name(_file_name), start_offset(_start_offset),
        end_offset(_end_offset), length(_length) {}

    std::string file_name;
    uint32_t start_offset;
    std::optional<uint32_t> end_offset, length;
};

int start(Input input);