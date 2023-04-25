#include "generator.hpp"

#include <fstream>

void generate_statement_list(std::ofstream &out, Node *node)
{
    for (auto statement : node->children)
    {
        if (statement->type == NodeType::PRINT_STATEMENT)
        {
            out << "std::cout << " << statement->children[0]->value << " << std::endl;" << std::endl;
        }
        if (statement->type == NodeType::RETURN_STATEMENT)
        {
            out << "return " << statement->children[0]->value << ";" << std::endl;
        }
        if (statement->type == NodeType::DECLARATION) {
            auto type = "";
            if (statement->children[0]->type == NodeType::TYPE_INT) {
                type = "int";
            }
            auto identifier = statement->children[1]->value;
            auto expression = statement->children[2]->value;
            out << type << " " << identifier << " = " << expression << ";" << std::endl;
        }
    }
}

void generate_block(std::ofstream &out, Node *node)
{
    out << "{" << std::endl;
    generate_statement_list(out, node->children[0]);
    out << "}" << std::endl;
}

void generate_function(std::ofstream &out, Node *node)
{
    auto type = "";
    if (node->children[0]->type == NodeType::TYPE_INT)
    {
        type = "int";
    }

    auto func_name = node->children[1]->value;

    out << type << " " << func_name << "()" << std::endl;
    auto block = node->children[2];
    generate_block(out, block);
}

void generate_code(Node *node, const std::string &filename)
{
    std::ofstream out(filename);

    out << "#include <iostream>" << std::endl;

    for (auto child : node->children)
    {
        if (child->type == NodeType::FUNCTION)
        {
            generate_function(out, child);
        }
    }
}

void compile_code(const std::string &ir, const std::string &filename)
{
    std::string command = "clang++ -std=c++17 -O3 -o " + filename + " " + ir;
    system(command.c_str());
}