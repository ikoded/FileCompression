#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <memory>
#include <vector>

struct Node {
    char ch;
    int freq;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

struct CompareNodes {
    bool operator()(const std::shared_ptr<Node>& a,const std::shared_ptr<Node>& b){
        // this will return lowest to greatest freq
        return a->freq > b->freq;
    }
};

/*

Generate the code recursively using the parent node, code and the codes reference to map out each char

Going left will add a 0, going right will add a 1

*/
void generate_huffman_codes(const std::shared_ptr<Node>& root, const std::string& code, std::unordered_map<char, std::string>& codes){
    // does not have left/right so this means it is a leaf not a root
    if(!root->left && !root->right){
        codes[root->ch] = code;
    }else{ // has left/right so need to traverse more
        generate_huffman_codes(root->left, code + "0", codes);
        generate_huffman_codes(root->right, code + "1", codes);
    }
}

/*

Encode the text with the generated huffman codes for each character

*/
std::string encode_text(std::string text, const std::unordered_map<char,std::string> codes){
    std::string encoded_text = "";
    for(char c : text){
        encoded_text.append(codes.at(c));
    }

    return encoded_text;
}

/*

The idea for decoding is we always know the first character in the encoded text will be one of the codes 
[0,n], where n is the length of the code for a character in the codes map.

As we erase from index 0, this means the first character will ALWAYS be a start to one of the codes generated
for each character.

*/
std::string decode_text(std::string encoded_text, const std::unordered_map<char,std::string> codes){
    std::string decoded_text;

    // while greater than 0 because the set of characters get deleted as it finds them
    while(encoded_text.length() > 0){
        // go through each code
        for(auto const& [key,value] : codes){
            // find length of the source to replace
            int size_code = value.length();
            // always starts at 0 index since it erases if found
            if(encoded_text.substr(0,size_code) == value){
                // erase the substring
                encoded_text.erase(0,size_code);
                // convert the character to a string and append to the decoded text string
                std::string tempchar(1,key);
                decoded_text.append(tempchar);
                // no longer need to look at other codes
                break;
            }
        }
    }
    return decoded_text;
}

int main() {
    std::string text = "This is a file compression using the Huffman Coding algorithm.";
    std::unordered_map<char,int> frequencies;

    // Grab frequency of characters in text
    for(char c : text){
        frequencies[c]++;
    }

    // create a min heap, lowest to greatest
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNodes> minheap;
    for(auto const& [key,value] : frequencies){
        minheap.push(std::make_shared<Node>(key,value));
    }

    // build the tree
    // get the left/right (min start) then add those frequencies
    // this creates a parent node that gets added with the next
    // say A(1) B(2) C(3) then the first iteration will have min heap like:
    // C(3) PARENT(3) where PARENT char is the dummy value \0
    // this can continue for a whole binary tree to get 1 parent node at root
    while(minheap.size() > 1){
        auto left_node = minheap.top();
        minheap.pop();
        auto right_node = minheap.top();
        minheap.pop();

        // add a parent node to the minheap, the ch is a dummy value while freq is left + right freq 
        auto parent_node = std::make_shared<Node>('\0',(left_node->freq + right_node->freq));
        // link the left and right of this node in the struct of parent node
        parent_node->left = left_node;
        parent_node->right = right_node;

        minheap.push(parent_node);
    }

    // grab root which is parent node of last frequencies added
    auto root = minheap.top();

    // generate the codes for each char based on nodes
    std::unordered_map<char, std::string> codes;
    generate_huffman_codes(root,"",codes);

    std::cout << "Original text: " << text << std::endl;

    std::string encoded_text = encode_text(text,codes);
    std::cout << "Encoded text: " << encoded_text << std::endl;

    std::string decoded_text = decode_text(encoded_text,codes);
    std::cout << "Decoded text: " << decoded_text << std::endl;


    return 0;
}