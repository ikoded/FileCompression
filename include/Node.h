#pragma once
#include <memory>

class Node{
    public:
        // getters & setters
        int get_freq();
        void set_freq(int freq);

        char get_ch();
        void set_ch(char ch);

        std::shared_ptr<Node> get_left_or_right(int direction); // 0 for left, 1 for right
        void set_left_or_right(int direction, std::shared_ptr<Node> node);

        struct CompareNode;

        Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr){}
    private:
        int freq;
        char ch;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
};