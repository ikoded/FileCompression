#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <queue>
#include <vector>
#include "Node.h"
#include "File.h"

class HuffmanCode{
    public:
        static void start_huffman_encoding();
        static void generate_huffman_codes(const std::shared_ptr<Node>& root, const std::string& code, std::unordered_map<char, std::string>& codes);
        static std::string encode_text(std::string text, const std::unordered_map<char, std::string> codes);
        static std::string decode_text(std::string encoded_text, const std::unordered_map<char, std::string> codes);
};