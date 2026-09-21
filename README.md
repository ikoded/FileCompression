# FileCompression
C++ command line tool to compress files using Huffman Coding

## How to use

Run `cmake -B build -S .` and `cmake --build build -j`. This will produce a executable in `build/`, run this with `./build/compression`.

This CLI has two options, `--compress` and `--decompress`. The `--compress` option will compress any txt file in `data_in` to `FILENAME.bin` where `FILENAME` is the original txt file's name. The `--decompress` option will decompress any bin file in `data_out` to the `data_in` to `FILENAME.txt` where `FILENAME` is the original bin file's name. The bin file in `data_out` must've been compressed by this CLI so it can correctly decode.

## How/Why Huffman Coding

### Why?

Each char in a text file is stored within 1 byte which is 8 bits. To tackle this issue, Huffman Coding uses a min heap and frequency map to decrease the amount of bits stored in a compressed file.

Example: `ABC` can be mapped to A(0) B(10) C(11). `ABC` would be 3 bytes or 24 bits stored uncompressed. However when compressed, it will be stored as 01011 which is only 5 bits.

While 24 -> 5 bits is a very good optimization, you also need to spend a few more bytes in the header of the compressed file for the decoding program to work. This of course defeats the purpose of compressing very small texts like `ABC` but in longer texts it saves much more space as long as it is greater than the header overhead in size.

### How?

Huffman Coding is quite simple logically. The steps are as follows:

1. Get the frequency of each character in a text and store it.
2. Create a min heap of the frequency, to start at lowest frequency characters.
3. Grab the left and right leaf nodes (first/second indices of minheap) and create a parent node adding the frequencies pushing it to end of minheap.
4. Recursively generate the huffman codes based on where the leaf node is, going left append `0` and going right append `1`.
5. Convert each character in the original text to the respective code generated in step 4.

As you can see, it uses a basic Binary Tree and the Binary Tree Search algorithm.

## Header Serialization

a binary header needs to tell the decompression program how to decode it.

Use count-based:

```
4 Bytes (Number of 8 bit pairs to go through) +
4 bytes (Number of bits leftover not in perfect 8 bit pair) +
4 Bytes (Size of unique characters) + 
(LOOP)
1 Byte (char) +
N Byte (codes can be 1-N size, N being max code char* size) + 
(END LOOP)
Rest of Bytes (Compressed bits code)
```