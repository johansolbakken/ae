#include "generator.hpp"
#include "aec.hpp"

#include <fstream>
#include <iostream>

std::string node_type_to_cpp(Node* node)
{
    switch (node->type)
    {
    case NodeType::TYPE_VOID:
        return "void";
    case NodeType::TYPE_BOOL:
        return "bool";
    case NodeType::TYPE_I8:
        return "int8_t";
    case NodeType::TYPE_I16:
        return "int16_t";
    case NodeType::TYPE_I32:
        return "int32_t";
    case NodeType::TYPE_I64:
        return "int64_t";
    case NodeType::TYPE_U8:
        return "uint8_t";
    case NodeType::TYPE_U16:
        return "uint16_t";
    case NodeType::TYPE_U32:
        return "uint32_t";
    case NodeType::TYPE_U64:
        return "uint64_t";
    case NodeType::TYPE_F32:
        return "float";
    case NodeType::TYPE_F64:
        return "double";
    case NodeType::TYPE_POINTER:
        return node_type_to_cpp(node->children[0]) + "*";
    default:
        break;
    }

    std::cout << node_type_to_string(node->type) << std::endl;

    assert(false && "type overflow");
    return "";
}

void generate_type(std::ofstream &out, Node *node)
{
    if (node->type == NodeType::TYPE)
    {
        return generate_type(out, node->children[0]);
    }
    out << node_type_to_cpp(node);
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
    auto func_name = node->children[1]->value;

    if (func_name == "main") {
        out << "int32_t";
    } else {
        generate_type(out, node->children[0]);
    }

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
    out << "#include <cstdint>" << std::endl;

    generate_program(out, node);
}

void compile_code(const std::string &ir, const std::string &filename)
{
    std::string command = "clang++ -std=c++17 -O3 -o " + filename + " " + ir;
    system(command.c_str());
}