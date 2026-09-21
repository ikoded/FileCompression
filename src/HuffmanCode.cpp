#include "../include/HuffmanCode.h"

void HuffmanCode::start_huffman_encoding(){
    std::unordered_map<char,int> frequencies;
    File file;
    std::vector<std::string> files;

    // grab files in data_in
    files = file.get_files("data_in");

    while(files.size() != 0){
        // get file name and pop out
        std::string filename = files.back();
        files.pop_back();

        std::string file_content = file.read_file(filename);

        // grab the frequencies of each character
        for(char c : file_content){
            frequencies[c]++;
        }

        // create a min heap of lowest to greatest
        std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, Node::CompareNode> minheap;
        for(const auto& [ch,freq] : frequencies){
            minheap.push(std::make_shared<Node>(ch,freq));
        }

        // go through minheap, creating dummy parent nodes that have \0 as the character and frequency set as left + right frequencies
        while(minheap.size() > 1){
            auto left_node = minheap.top();
            minheap.pop();

            auto right_node = minheap.top();
            minheap.pop();

            auto parent_node = std::make_shared<Node>('\0',(left_node->get_freq() + right_node->get_freq()));
            // set left and right leaf nodes for parent node
            parent_node->set_left_or_right(0,left_node);
            parent_node->set_left_or_right(1,right_node);

            minheap.push(parent_node);
        }

        // grab the root and start the code generation
        auto root = minheap.top();

        // generate huffman codes
        std::unordered_map<char,std::string> codes;
        generate_huffman_codes(root, "", codes);
        // encode text with huffman codes
        std::string encoded_text = encode_text(file_content,codes);

        // write bin file to data_out
        file.compress_file(filename,encoded_text,codes);
        file.compare_sizes(filename);
    }
}

void HuffmanCode::generate_huffman_codes(const std::shared_ptr<Node>& root, const std::string& code, std::unordered_map<char, std::string>& codes){
    if(!root->get_left_or_right(0) && !root->get_left_or_right(1)){
        codes[root->get_ch()] = code;
    }else{
        generate_huffman_codes(root->get_left_or_right(0), code + "0", codes);
        generate_huffman_codes(root->get_left_or_right(1), code + "1", codes);
    }
}

/*

Manual Compression and Decompression

*/
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