#include "../include/File.h"

std::string File::read_file(std::string filename){
    std::string file_content = "";
    try{
        std::ifstream file_in("data_in/" + filename + ".txt");
        std::string line;
        
        while(getline(file_in,line)){
            file_content += line + "\n";
        }
    }catch(const std::runtime_error& e){
        std::cerr << "Runtime error: " << e.what() << std::endl;
    }catch(const std::exception& e){
        std::cerr << "Exception error: " << e.what() << std::endl;
    }
    
    return file_content;
}

void File::compress_file(std::string filename, std::string encoded_text){
    std::string file_out = "data_out/" + filename + ".bin";
    try{
        std::ofstream compressed_bin(file_out, std::ios::binary);
        compressed_bin.write(encoded_text.c_str(),encoded_text.length());
    }catch(const std::runtime_error& e){
        std::cerr << "Runtime error: " << e.what() << std::endl;
    }catch(const std::exception& e){
        std::cerr << "Exception error: " << e.what() << std::endl;
    }
}

void File::compare_sizes(std::string filename){
    std::string file_in = "data_in/" + filename + ".txt";
    std::string file_out = "data_out/" + filename + ".bin";

    std::cout << "Original size: " << std::filesystem::file_size(file_in) << " bytes" << std::endl;
    std::cout << "Compressed size: " << std::filesystem::file_size(file_out) << " bytes" << std::endl;
}