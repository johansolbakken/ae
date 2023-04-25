#pragma once

#include <string>
#include "tree.hpp"

void generate_code(Node* node, const std::string& filename);
void compile_code(const std::string& ir, const std::string& filename);