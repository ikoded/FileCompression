#pragma once
#include <string>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <filesystem>

class File {
    public:
        std::string read_file(std::string filename);
        void compress_file(std::string filename, std::string encoded_text);
        std::string decompress_file(std::string filename);
        void compare_sizes(std::string filename);
};