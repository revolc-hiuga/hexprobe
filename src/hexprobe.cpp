#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <optional>
#include <filesystem>
#include <system_error>
#include <fstream>
#include <limits>
#include <exception>
#include "hexprobe.h"
#include "error.h"
#include "display.h"

struct LogicalInput {
    LogicalInput(std::vector<uint8_t> _buffer, uint32_t _start_offset, uint32_t _length):
        buffer(_buffer), start_offset(_start_offset), length(_length) {}

    std::vector<uint8_t> buffer;
    uint32_t start_offset, length;
};

struct ConvertResult {
    ConvertResult(int _status_code, std::optional<LogicalInput> _l_input_opt):
        status_code(_status_code), l_input_opt(_l_input_opt) {}

    int status_code;
    std::optional<LogicalInput> l_input_opt;
};

ConvertResult convert_input(Input input);
bool is_file_exist(std::string file_name);
size_t get_file_size(std::string file_name);
bool read_file(std::string file_name, uint32_t start_offset, uint32_t length, std::vector<uint8_t> &buffer);

int start(Input input) {
    ConvertResult convert_result = convert_input(input);
    if(convert_result.status_code != StatusCode::OK) {
        print_errmsg(convert_result.status_code);
        return 2;
    }

    LogicalInput l_input = *convert_result.l_input_opt;
    print_header(l_input.start_offset, l_input.length);
    print_table(l_input.buffer, l_input.start_offset, l_input.length);

    return 0;
}

ConvertResult convert_input(Input input) {
    std::string file_name = input.file_name;
    if(!is_file_exist(file_name)) {
        return ConvertResult(StatusCode::FileIsNotExist, std::nullopt);
    }

    size_t file_size = get_file_size(file_name);
    if(file_size == 0) {
        return ConvertResult(StatusCode::FileIsEmpty, std::nullopt);
    }

    uint32_t length = 0, start_offset = input.start_offset;
    if(!input.end_offset && !input.length) {
        if(file_size-1 < start_offset) {
            return ConvertResult(StatusCode::OverRange, std::nullopt);
        }
        
        length = file_size-start_offset-1;
        if(length > 256) {
            length = 256;
        }
    } else if(!input.length) {
        if(*input.end_offset <= start_offset) {
            return ConvertResult(StatusCode::RangeIsNotExist, std::nullopt);
        }
        length = *input.end_offset - start_offset;
    } else if(!input.end_offset) {
        length = *input.length;
    }

    if(length > 1024) {
        return ConvertResult(StatusCode::RangeIsTooLong, std::nullopt);
    } else if(length == 0) {
        return ConvertResult(StatusCode::RangeIsNotExist, std::nullopt);
    }

    if(start_offset + length > file_size-1) {
        return ConvertResult(StatusCode::OverRange, std::nullopt);
    }

    std::vector<uint8_t> buffer;
    if(!read_file(file_name, start_offset, length, buffer)) {
        return ConvertResult(StatusCode::ReadFileError, std::nullopt);
    }

    return ConvertResult(StatusCode::OK, LogicalInput(buffer, start_offset, length));
}

bool is_file_exist(std::string file_name) {
    std::error_code error;
    return std::filesystem::exists(file_name, error);
}

size_t get_file_size(std::string file_name) {
    std::error_code error;
    const auto size = std::filesystem::file_size(file_name, error);
    if (error || size > std::numeric_limits<size_t>::max()) {
        return 0;
    }
    return static_cast<size_t>(size);
}

bool read_file(std::string file_name, uint32_t start_offset, uint32_t length, std::vector<uint8_t> &buffer) {
    try {
        if (start_offset > static_cast<uintmax_t>(std::numeric_limits<std::streamoff>::max()) ||
            length > static_cast<uintmax_t>(std::numeric_limits<std::streamsize>::max())) {
            return false;
        }

        std::ifstream file(file_name, std::ios::binary);
        if (!file || !file.seekg(static_cast<std::streamoff>(start_offset), std::ios::beg)) {
            return false;
        }

        buffer.resize(length);
        if (length == 0) {
            return true;
        }
        return static_cast<bool>(file.read(reinterpret_cast<char *>(buffer.data()),
                                           static_cast<std::streamsize>(length)));
    } catch (const std::exception &) {
        return false;
    }
}
