#include "../include/File.h"
#include "../include/Node.h"
#include "../include/HuffmanCode.h"

#include <unordered_map>
#include <queue>
#include <vector>

struct CompareNode{
    bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b){
        return a->get_freq() > b->get_freq(); // lowest to greatest
    }
};


int main(int argc, char* argv[]){
    std::unordered_map<char,int> frequencies;
    File file;

    // error check command line commands
    if(argc != 2){
        std::cerr << "Incorrect arguments, please review README. Arguments provided: " << std::endl;
        for(int i = 0; i < argc; ++i){
            std::cerr << "- " << argv[i] << std::endl;
        }
        return 1;
    }

    std::string filename = argv[1];
    std::string file_content = file.read_file(filename);
    if(file_content == ""){
        std::cerr << "Please make sure file exists or is in the data_in folder." << std::endl;
        return 1;
    }

    // grab the frequencies of each character
    for(char c : file_content){
        frequencies[c]++;
    }

    // create a min heap of lowest to greatest
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNode> minheap;
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
    HuffmanCode::generate_huffman_codes(root, "", codes);

    std::cout << "Original content:\n" << file_content << "\n\n" << std::endl;
    std::string encoded_text = HuffmanCode::encode_text(file_content, codes);
    // just for fun, nothing is done with this variable
    std::string decoded_text = HuffmanCode::decode_text(encoded_text, codes);

    file.compress_file(filename, encoded_text);
    std::string decompressed_content = file.decompress_file(filename);

    if(decompressed_content == ""){
        std::cerr << "Error decompressing file." << std::endl;
        return 1;
    }

    std::cout << "Decoded text from bin:\n" << HuffmanCode::decode_text(decompressed_content, codes) << "\n\n" << std::endl;

    file.compare_sizes(filename);

    return 0;
}