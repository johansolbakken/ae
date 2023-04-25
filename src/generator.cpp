#include "generator.hpp"

#include <fstream>

void generate_code(Node* node, const std::string& filename)
{
    std::ofstream out(filename);

    out << "#include <iostream>" << std::endl;

    for (auto child : node->children)
    {
        if (child->type == NodeType::FUNCTION) {
            out << "int " << child->children[1]->value << "() {" << std::endl;
            auto block = child->children[2];
            for (auto statement : block->children[0]->children)
            {
                if (statement->type == NodeType::PRINT_STATEMENT) {
                    out << "std::cout << " << statement->children[0]->value << " << std::endl;" << std::endl;
                }
                if (statement->type == NodeType::RETURN_STATEMENT) {
                    out << "return " << statement->children[0]->value << ";" << std::endl;
                }
            }
            out << "}" << std::endl;
        }
    }
}

void compile_code(const std::string& ir, const std::string& filename)
{
    std::string command = "clang++ -std=c++17 -O3 -o " + filename + " " + ir;
    system(command.c_str());
}