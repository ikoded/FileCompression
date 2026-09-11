#include "../include/HuffmanCode.h"

void HuffmanCode::generate_huffman_codes(const std::shared_ptr<Node>& root, const std::string& code, std::unordered_map<char, std::string>& codes){
    if(!root->get_left_or_right(0) && !root->get_left_or_right(1)){
        codes[root->get_ch()] = code;
    }else{
        generate_huffman_codes(root->get_left_or_right(0), code + "0", codes);
        generate_huffman_codes(root->get_left_or_right(1), code + "1", codes);
    }
}

std::string HuffmanCode::encode_text(std::string text, const std::unordered_map<char, std::string> codes){
    std::string encoded_text = "";
    for(char c : text){
        encoded_text.append(codes.at(c));
    }

    return encoded_text;
}

std::string HuffmanCode::decode_text(std::string encoded_text, const std::unordered_map<char, std::string> codes){
    std::string decoded_text = "";
    while(encoded_text.length() > 0){
        for(const auto& [ch,code] : codes){
            int size_code = code.length();

            if(encoded_text.substr(0,size_code) == code){
                encoded_text.erase(0,size_code);

                std::string tempch(1,ch);
                decoded_text.append(tempch);

                break;
            }
        }
    }

    return decoded_text;
}