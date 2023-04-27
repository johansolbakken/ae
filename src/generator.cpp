#include "generator.hpp"
#include "aec.hpp"

#include <fstream>
#include <iostream>

std::string node_type_to_cpp(Node *node)
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

void generate_expression_list(std::ofstream &out, Node *node);
void generate_call(std::ofstream &out, Node *node)
{
    auto identifier = node->children[0]->value;
    out << identifier << "(";
    generate_expression_list(out, node->children[1]);
    out << ")";
}

void generate_array_indexing(std::ofstream &out, Node *node);
void generate_expression(std::ofstream &out, Node *node)
{
    if (node->type == NodeType::CALL_STATEMENT)
    {
        generate_call(out, node);
    }
    else if (node->type == NodeType::INITIALIZER)
    {
        out << "{}" << std::endl;
    }
    else if (node->type == NodeType::ARRAY_INDEXING)
    {
        generate_array_indexing(out, node);
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

void generate_variable_declaration(std::ofstream &out, Node *node)
{
    generate_type(out, node->children[0]);
    auto identifier = node->children[1]->value;
    out << " " << identifier << " = ";
    generate_expression(out, node->children[2]);
    out << ";" << std::endl;
}

void generate_array_declaration(std::ofstream &out, Node *node)
{
    generate_type(out, node->children[0]);
    auto identifier = node->children[2]->value;
    out << " " << identifier << "[";
    generate_expression(out, node->children[1]);
    out << "] = ";
    generate_expression(out, node->children[3]);
    out << ";" << std::endl;
}

void generate_array_indexing(std::ofstream &out, Node *node)
{
    auto identifier = node->children[0]->value;
    out << identifier << "[";
    generate_expression(out, node->children[1]);
    out << "]";
}

void generate_expression_list(std::ofstream &out, Node *node)
{
    if (node->type == NodeType::EXPRESSION)
    {
        generate_expression(out, node);
        return;
    }
    assert(node->type == NodeType::EXPRESSION_LIST);
    for (int i = 0; i < node->children.size(); i++)
    {
        generate_expression(out, node->children[i]);
        if (i != node->children.size() - 1)
        {
            out << ", ";
        }
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
    if (node->type == NodeType::VARIABLE_DECLARATION)
    {
        generate_variable_declaration(out, node);
    }
    if (node->type == NodeType::ARRAY_DECLARATION)
    {
        generate_array_declaration(out, node);
    }
    if (node->type == NodeType::ASSIGNMENT_STATEMENT)
    {
        if (node->value == "*")
        {
            // Pointer set
            out << "*";
            generate_expression(out, node->children[0]);
        }
        else if (node->children[0]->type == NodeType::ARRAY_INDEXING)
        {
            generate_array_indexing(out, node->children[0]);
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
        generate_call(out, node);
        out << ";" << std::endl;
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

void generate_parameter_list(std::ofstream &out, Node *node)
{
    assert(node->type == NodeType::PARAMETER_LIST || node->type == NodeType::PARAMETER);
    if (node->type == NodeType::PARAMETER)
    {
        generate_type(out, node->children[0]);
        out << " " << node->children[1]->value;
        return;
    }
    for (int i = 0; i < node->children.size(); i++)
    {
        auto child = node->children[i];
        generate_type(out, child->children[0]);
        out << " " << child->children[1]->value;
        if (i != node->children.size() - 1)
        {
            out << ", ";
        }
    }
}

void generate_function(std::ofstream &out, Node *node)
{
    auto func_name = node->children[1]->value;

    if (func_name == "main")
    {
        out << "int32_t";
    }
    else
    {
        generate_type(out, node->children[0]);
    }

    out << " " << func_name << "(";
    generate_parameter_list(out, node->children[2]);
    out << ")" << std::endl;
    auto block = node->children[3];
    generate_block(out, block);
}

void generate_program(std::ofstream &out, Node *node)
{
    if (node->type == NodeType::GLOBAL_LIST)
    {
        for (auto child : node->children)
        {
            generate_program(out, child);
        }
    }

    if (node->type == NodeType::VARIABLE_DECLARATION)
    {
        generate_variable_declaration(out, node);
    }

    if (node->type == NodeType::ARRAY_DECLARATION)
    {
        generate_array_declaration(out, node);
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