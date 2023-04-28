#pragma once

#include <string>
#include "tree.hpp"
#include "arch/arch.hpp"

void generate_code(Node *node, const std::string &filename, const std::shared_ptr<Arch> &generator);
void compile_code(const std::string &ir, const std::string &filename);