#pragma once

#include <cstdint>
#include <vector>

void print_header(uint32_t start_offset, uint32_t length);
void print_table(std::vector<uint8_t> &buffer, uint32_t start_offset, uint32_t length);