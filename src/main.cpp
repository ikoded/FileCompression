#include "../include/File.h"
#include "../include/Node.h"
#include "../include/HuffmanCode.h"

int parse_args(int argc, char* argv[]){
    // error check command line commands
    if(argc != 2){
        std::cerr << "Incorrect amount of arguments, please review README. Arguments provided: " << std::endl;
        for(int i = 0; i < argc; ++i){
            std::cerr << "- " << argv[i] << std::endl;
        }
        return 0;
    }

    std::string argument_choice = argv[1];

    if(argument_choice == "--compress"){
        return 1; // compress option
    }else if(argument_choice == "--decompress"){
        return 2; // decompress option
    }else{
        // invalid choice for argument
        std::cerr << "Only valid argument options are `--compress` and `--decompress`." << std::endl;
        return 0;
    }
}

int main(int argc, char* argv[]){
    int choice = parse_args(argc,argv);
    switch(choice){
        case 0: {
            // this means args passed was wrong in some way, error message in parse_args
            return 1;
            break;
        }
        case 1: {
            // this means compress files in data_in
            HuffmanCode::start_huffman_encoding();
            break;
        }
        case 2: {
            // this means decompress files in data_out
            File file;
            file.decompress_files();
            break;
        }
        default: {
            break;
        }
    }

    return 0;
}