#pragma once
#include <memory>

class Node{
    public:
        // getters & setters
        int get_freq();

        char get_ch();

        std::shared_ptr<Node> get_left_or_right(int direction); // 0 for left, 1 for right
        void set_left_or_right(int direction, std::shared_ptr<Node> node);

        struct CompareNode{
            bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b){
                return a->get_freq() > b->get_freq(); // lowest to greatest
            }
        };


        Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr){}
    private:
        int freq;
        char ch;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
};