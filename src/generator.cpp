#include "generator.hpp"
#include "aec.hpp"
#include "arch/arch_cpp.hpp"

#include <fstream>
#include <iostream>

void generate_code(Node *node, const std::string &filename)
{
    std::ofstream out(filename);

    ArchCpp generator;
    generator.gen_header(out, nullptr);
    generator.gen_program(out, node);
}

void compile_code(const std::string &ir, const std::string &filename)
{
    std::string command = "clang++ -std=c++17 -O3 -o " + filename + " " + ir;
    system(command.c_str());
}