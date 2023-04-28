#include "generator.hpp"
#include "aec.hpp"
#include "arch/arch_cpp.hpp"

#include <fstream>
#include <iostream>
#include <memory>

void generate_code(Node *node, const std::string &filename, const std::shared_ptr<Arch> &generator)
{
    std::ofstream out(filename);
    generator->gen_header(out, nullptr);
    generator->gen_program(out, node);
    generator->gen_footer(out, nullptr);
    out.close();
}

void compile_code(const std::string &ir, const std::string &filename)
{
    std::string command = "clang++ -std=c++17 -O3 -o " + filename + " " + ir;
    system(command.c_str());
}