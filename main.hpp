#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <zlib.h>

struct files_info
{
    std::string path;
    size_t size_in;
    size_t size_out;
    files_info(std::string _path, size_t _size_in, size_t _size_out ):path(_path),size_in(_size_in),size_out(_size_out){}
};