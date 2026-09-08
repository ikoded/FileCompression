# FileCompression
C++ command line tool to compress files using Huffman Coding

## Header Serialization

Binary header needs to tell the decompression program how to decode it

Use count-based:

4 Bytes (Size of unique characters) + 
5 Bytes (char + frequency of char int(4 bytes)) + 
Rest of Bytes (Compressed bits code)