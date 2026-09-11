#include "../include/Node.h"

int Node::get_freq(){
    return freq;
}

char Node::get_ch(){
    return ch;
}


std::shared_ptr<Node> Node::get_left_or_right(int direction){
    if(direction==0){
        return left;
    }else{
        return right;
    }
}

void Node::set_left_or_right(int direction, std::shared_ptr<Node> node){
    if(direction==0){
        this->left = node;
    }else{
        this->right = node;
    }
}


