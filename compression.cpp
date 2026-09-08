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

int main() {
    std::string text = "AABCBAD";
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

    // print values for now, in future will add decoder as well as serialize through headers
    for(auto const& [key,value] : codes){
        std::cout << key << " " << value << std::endl;
    }


    return 0;
}