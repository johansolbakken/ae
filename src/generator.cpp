#include "generator.hpp"

#include <fstream>

void generate_type(std::ofstream &out, Node *node)
{
    if (node->type == NodeType::TYPE_INT)
    {
        out << "int";
    }
    else if (node->type == NodeType::TYPE_FLOAT)
    {
        out << "float";
    }
    else if (node->type == NodeType::TYPE_VOID)
    {
        out << "void";
    }
    else if (node->type == NodeType::POINTER)
    {
        generate_type(out, node->children[0]);
        out << "*";
    }
}

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
        if (node->value == "*")
        {
            out << "*";
        }
        else if (node->value == "&")
        {
            out << "&";
        }
        else if (node->value == "-")
        {
            out << "-";
        }

        generate_expression(out, node->children[0]);
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
        out << "std::cout << ";
        generate_expression(out, node->children[0]);
        if (node->value == "\n")
        {
            out << " << std::endl";
        }
        out << ";";
        out << std::endl;
    }
    if (node->type == NodeType::RETURN_STATEMENT)
    {
        out << "return ";
        generate_expression(out, node->children[0]);
        out << ";" << std::endl;
    }
    if (node->type == NodeType::DECLARATION)
    {
        generate_type(out, node->children[0]);
        auto identifier = node->children[1]->value;
        out << " " << identifier << " = ";
        generate_expression(out, node->children[2]);
        out << ";" << std::endl;
    }
    if (node->type == NodeType::ASSIGNMENT_STATEMENT)
    {
        if (node->value == "*")
        {
            // Pointer set
            out << "*";
            generate_expression(out, node->children[0]);
        }
        else
        {
            // Normal set
            auto identifier = node->children[0]->value;
            out << identifier;
        }

        out << " = ";
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
    generate_type(out, node->children[0]);

    auto func_name = node->children[1]->value;

    out << " " << func_name << "()" << std::endl;
    auto block = node->children[2];
    generate_block(out, block);
}

void generate_program(std::ofstream &out, Node *node)
{
    for (auto child : node->children)
    {
        generate_program(out, child);
    }

    if (node->type == NodeType::FUNCTION)
    {
        generate_function(out, node);
    }
}

void generate_code(Node *node, const std::string &filename)
{
    std::ofstream out(filename);

    out << "#include <iostream>" << std::endl;

    generate_program(out, node);
}

void compile_code(const std::string &ir, const std::string &filename)
{
    std::string command = "clang++ -std=c++17 -O3 -o " + filename + " " + ir;
    system(command.c_str());
}