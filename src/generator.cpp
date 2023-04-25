#include "generator.hpp"

#include <fstream>

void generate_condition(std::ofstream &out, Node *node);
void generate_expression(std::ofstream &out, Node *node);
void generate_statement_list(std::ofstream &out, Node *node);
void generate_statement(std::ofstream &out, Node *node);
void generate_while(std::ofstream &out, Node *node)
{
    out << "while (";
    generate_condition(out, node->children[0]);
    out << ") {" << std::endl;
    generate_statement_list(out, node->children[1]);
    out << "}" << std::endl;
}

void generate_condition(std::ofstream &out, Node *node)
{
    if (node->children.size() == 1)
    {
        out << node->children[0]->value;
        return;
    }

    generate_expression(out, node->children[0]);
    out << " " << node->value << " ";
    generate_expression(out, node->children[1]);
}

void generate_expression(std::ofstream &out, Node *node)
{
    if (node->type == NodeType::CALL_STATEMENT)
    {
        out << node->children[0]->value << "()";
        return;
    }
    else if (node->children.size() == 1)
    {
        out << node->children[0]->value;
    }
    else if (node->children.size() == 2)
    {
        generate_expression(out, node->children[0]);
        out << " " << node->value << " ";
        generate_expression(out, node->children[1]);
    }
    else
    {
        out << node->value;
    }
}

void generate_statement(std::ofstream &out, Node *node)
{
    if (node->type == NodeType::PRINT_STATEMENT)
    {
        out << "std::cout << " << node->children[0]->value << " << std::endl;" << std::endl;
    }
    if (node->type == NodeType::RETURN_STATEMENT)
    {
        out << "return " << node->children[0]->value << ";" << std::endl;
    }
    if (node->type == NodeType::DECLARATION)
    {
        auto type = "";
        if (node->children[0]->type == NodeType::TYPE_INT)
        {
            type = "int";
        }
        auto identifier = node->children[1]->value;
        out << type << " " << identifier << " = ";
        generate_expression(out, node->children[2]);
        out << ";" << std::endl;
    }
    if (node->type == NodeType::ASSIGNMENT_STATEMENT)
    {
        auto identifier = node->children[0]->value;
        out << identifier << " = ";
        generate_expression(out, node->children[1]);
        out << ";" << std::endl;
    }
    if (node->type == NodeType::CALL_STATEMENT)
    {
        auto identifier = node->children[0]->value;
        out << identifier << "();" << std::endl;
    }
    if (node->type == NodeType::IF_STATEMENT)
    {
        if (node->children.size() == 2)
        {
            out << "if (";
            generate_condition(out, node->children[0]);
            out << ") {" << std::endl;
            generate_statement_list(out, node->children[1]);
            out << "}" << std::endl;
        }
        else
        {
            out << "if (";
            generate_condition(out, node->children[0]);
            out << "){" << std::endl;
            generate_statement_list(out, node->children[1]);
            out << "}else{" << std::endl;
            generate_statement_list(out, node->children[2]);
            out << "}" << std::endl;
        }
    }
    if (node->type == NodeType::WHILE_STATEMENT)
    {
        generate_while(out, node);
    }
}

void generate_statement_list(std::ofstream &out, Node *node)
{
    if (node->type != NodeType::STATEMENT_LIST)
    {
        generate_statement(out, node);
        return;
    }
    for (auto statement : node->children)
    {
        generate_statement(out, statement);
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
    else if (node->children[0]->type == NodeType::TYPE_VOID)
    {
        type = "void";
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