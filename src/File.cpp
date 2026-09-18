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

/*

Compress file using algorithm of storing each bit left to right as a full byte (char) for .bin

*/
void File::compress_file(std::string filename, std::string encoded_text, std::unordered_map<char, std::string> codes){
    std::string file_out = "data_out/" + filename + ".bin";
    try{
        std::ofstream compressed_bin(file_out, std::ios::out | std::ios::binary);

        // write the headers at the beginning for the decoder to know
        // number of chars there will be
        uint32_t num_chars = codes.size();
        compressed_bin.write(reinterpret_cast<char*>(&num_chars), sizeof(num_chars));
        // initialize code size for loop
        uint32_t code_size;
        for(const auto& [letter,code] : codes){
            // write the char first
            compressed_bin.write(&letter, sizeof(letter));
            // then get and write the code size
            code_size = code.size();
            compressed_bin.write(reinterpret_cast<char*>(&code_size), sizeof(code_size));
            // finally write the code
            compressed_bin.write(code.c_str(), code_size);
        }

        // grab number of pairs to go through and remainder of bits, then write to header
        uint32_t num_pairs = encoded_text.length() / 8;
        uint32_t last_bits = encoded_text.length() % 8;
        compressed_bin.write(reinterpret_cast<char*>(&num_pairs), sizeof(num_pairs));
        compressed_bin.write(reinterpret_cast<char*>(&last_bits), sizeof(last_bits));

        while(num_pairs>0){
            // initialize the byte to pack & current bits to go through
            uint8_t packed_byte = 0;
            std::string current_bits;

            // if last pair, make sure to grab exact last_bits left
            if(num_pairs==1 && last_bits > 0){
                current_bits = encoded_text.substr(0,last_bits);
                encoded_text.erase(0,last_bits);
            }else{
                current_bits = encoded_text.substr(0,8);
                encoded_text.erase(0,8);
            }

            // go through the current_bits grabbed
            for(char c : current_bits){
                // set temp bit based on 1/0 char
                uint8_t temp;
                temp = c == '1' ? 1 : 0;
                
                // shift packed bytes 1 to left and or the temp bit
                // this essentially adds a bit to the far right, moving each bit so it stores left to right
                packed_byte = (packed_byte << 1) | temp;
            }
            
            // write the packged_byte as a char in the compressed .bin file
            compressed_bin.write(reinterpret_cast<char*>(&packed_byte),sizeof(packed_byte));

            num_pairs--;
        }
    }catch(const std::runtime_error& e){
        std::cerr << "Runtime error: " << e.what() << std::endl;
    }catch(const std::exception& e){
        std::cerr << "Exception error: " << e.what() << std::endl;
    }
}

/*

Decompresses file using algorithm set in compression, decode it and return a String

*/
std::string File::decompress_file(std::string filename){
    std::string file_out_name = "data_out/" + filename + ".bin";
    std::string file_content = "";
    std::string decoded_text = "";
    try{
        std::ifstream file_in(file_out_name, std::ios::binary);
        // grab code headers to decipher at end
        std::unordered_map<char, std::string> codes;
        // number of chars
        uint32_t char_n;
        file_in.read(reinterpret_cast<char*>(&char_n), sizeof(char_n));

        // initialize variables grabbed in loop
        char character;
        uint32_t code_size;
        std::string code;
        for(int i = 0; i < char_n; ++i){
            // first grab the char
            file_in.read(&character, sizeof(character));

            // then grab the size of the code since it is dynamic
            file_in.read(reinterpret_cast<char*>(&code_size), sizeof(code_size));

            // resize string so it can store code
            code.resize(code_size);
            // lastly grab the code
            // notice data is used, this is a pointer to the string's buffer
            file_in.read(code.data(), code_size);

            // add to codes unordered map
            codes[character] = code;
        }

        // read the first 8 bytes which is 2 ints storing how many pairs and last bits
        uint32_t num_pairs;
        uint32_t last_bits;
        file_in.read(reinterpret_cast<char*>(&num_pairs), sizeof(num_pairs));
        file_in.read(reinterpret_cast<char*>(&last_bits), sizeof(last_bits));

        while(num_pairs>0){
            // initialize variables
            uint8_t packed_byte;
            int bit = 0;

            // read char* into uint8_t packed_byte
            file_in.read(reinterpret_cast<char*>(&packed_byte), sizeof(packed_byte));

            // go from left to right like the compression stored it
            for(int i = 7; i >= 0; --i){
                // grab a mask of 1 to & to get bit
                // so for 7, that will make a mask of 10000000
                uint8_t mask = 1 << i;
                // store as char to easily translate and write in string
                char bit = (packed_byte & mask) ? '1' : '0';

                // if last pair check remaining bits to go through
                if(num_pairs == 1 && last_bits > 0){
                    // if i > last bits - 1 (0 based index) skip, since they are padded zeroes
                    if(i>last_bits-1){
                        continue;
                    }
                }
                // convert char to string and write to file_content
                std::string temp(1,bit);
                file_content.append(temp);
            }

            num_pairs--;
        }

        // decode text to return
        decoded_text = HuffmanCode::decode_text(file_content,codes);
    }catch(const std::runtime_error& e){
        std::cerr << "Runtime error: " << e.what() << std::endl;
    }catch(const std::exception& e){
        std::cerr << "Exception error: " << e.what() << std::endl;
    }

    return decoded_text;
}

void File::output_decoded_content(std::string filename, std::string file_content){
    try{
        // file to write to
        std::ofstream file("data_in/" + filename + "out.txt");

        file << file_content;

    }catch(const std::runtime_error& e){
        std::cerr << "Runtime error: " << e.what() << std::endl;
    }catch(const std::exception& e){
        std::cerr << "Exception error: " << e.what() << std::endl;
    }
}

/*

Compare sizes of the file before and afer, for metrics

*/
void File::compare_sizes(std::string filename){
    std::string file_in = "data_in/" + filename + ".txt";
    std::string file_out = "data_out/" + filename + ".bin";

    std::cout << "Original size: " << std::filesystem::file_size(file_in) << " bytes" << std::endl;
    std::cout << "Compressed size: " << std::filesystem::file_size(file_out) << " bytes" << std::endl;
}