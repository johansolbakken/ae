#include "arch/arch_cpp.hpp"

#include "tree.hpp"
#include "nodetypes.hpp"

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

void ArchCpp::gen_header(std::ofstream &out, Node *node)
{
    out << "#include <iostream>" << std::endl;
    out << "#include <cstdint>" << std::endl;
}

void ArchCpp::gen_footer(std::ofstream &out, Node *node)
{
}

void ArchCpp::gen_program(std::ofstream &out, Node *node)
{
    if (node->type == NodeType::GLOBAL_LIST)
    {
        for (auto child : node->children)
        {
            gen_program(out, child);
        }
    }

    if (node->type == NodeType::VARIABLE_DECLARATION)
    {
        gen_variable_declaration(out, node);
    }

    if (node->type == NodeType::ARRAY_DECLARATION)
    {
        gen_array_declaration(out, node);
    }

    if (node->type == NodeType::FUNCTION)
    {
        gen_function(out, node);
    }
}

void ArchCpp::gen_type(std::ofstream &out, Node *node)
{
    if (node->type == NodeType::TYPE)
    {
        gen_type(out, node->children[0]);
    }
    else
    {
        out << node_type_to_cpp(node);
    }
}

void ArchCpp::gen_condition(std::ofstream &out, Node *node)
{
    if (node->children.size() == 1)
    {
        out << node->children[0]->value;
        return;
    }

    gen_expression(out, node->children[0]);
    out << " " << node->value << " ";
    gen_expression(out, node->children[1]);
}

void ArchCpp::gen_while(std::ofstream &out, Node *node)
{
    // assert((node->type == NodeType::WHILE) && "node is not a while");
    out << "while (";
    gen_condition(out, node->children[0]);
    out << ") {" << std::endl;
    gen_statement_list(out, node->children[1]);
    out << "}" << std::endl;
}

void ArchCpp::gen_call(std::ofstream &out, Node *node)
{
    auto identifier = node->children[0]->value;
    out << identifier << "(";
    gen_expression_list(out, node->children[1]);
    out << ")";
}

void ArchCpp::gen_array_indexing(std::ofstream &out, Node *node)
{
    auto identifier = node->children[0]->value;
    out << identifier << "[";
    gen_expression(out, node->children[1]);
    out << "]";
}

void ArchCpp::gen_variable_declaration(std::ofstream &out, Node *node)
{
    gen_type(out, node->children[0]);
    auto identifier = node->children[1]->value;
    out << " " << identifier << " = ";
    gen_expression(out, node->children[2]);
    out << ";" << std::endl;
}

void ArchCpp::gen_array_declaration(std::ofstream &out, Node *node)
{
    gen_type(out, node->children[0]);
    auto identifier = node->children[2]->value;
    out << " " << identifier << "[";
    gen_expression(out, node->children[1]);
    out << "] = ";
    gen_expression(out, node->children[3]);
    out << ";" << std::endl;
}

void ArchCpp::gen_expression_list(std::ofstream &out, Node *node)
{
    if (node->type == NodeType::EXPRESSION)
    {
        gen_expression(out, node);
        return;
    }
    assert(node->type == NodeType::EXPRESSION_LIST);
    for (int i = 0; i < node->children.size(); i++)
    {
        gen_expression(out, node->children[i]);
        if (i != node->children.size() - 1)
        {
            out << ", ";
        }
    }
}

void ArchCpp::gen_expression(std::ofstream &out, Node *node)
{
    if (node->type == NodeType::CALL_STATEMENT)
    {
        gen_call(out, node);
    }
    else if (node->type == NodeType::INITIALIZER)
    {
        out << "{}" << std::endl;
    }
    else if (node->type == NodeType::ARRAY_INDEXING)
    {
        gen_array_indexing(out, node);
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

        gen_expression(out, node->children[0]);
    }
    else if (node->children.size() == 2)
    {
        gen_expression(out, node->children[0]);
        out << " " << node->value << " ";
        gen_expression(out, node->children[1]);
    }
    else
    {
        out << node->value;
    }
}

void ArchCpp::gen_statement(std::ofstream &out, Node *node)
{
    if (node->type == NodeType::PRINT_STATEMENT)
    {
        out << "std::cout << ";
        gen_expression(out, node->children[0]);
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
        gen_expression(out, node->children[0]);
        out << ";" << std::endl;
    }
    if (node->type == NodeType::VARIABLE_DECLARATION)
    {
        gen_variable_declaration(out, node);
    }
    if (node->type == NodeType::ARRAY_DECLARATION)
    {
        gen_array_declaration(out, node);
    }
    if (node->type == NodeType::ASSIGNMENT_STATEMENT)
    {
        if (node->value == "*")
        {
            // Pointer set
            out << "*";
            gen_expression(out, node->children[0]);
        }
        else if (node->children[0]->type == NodeType::ARRAY_INDEXING)
        {
            gen_array_indexing(out, node->children[0]);
        }
        else
        {
            // Normal set
            auto identifier = node->children[0]->value;
            out << identifier;
        }

        out << " = ";
        gen_expression(out, node->children[1]);
        out << ";" << std::endl;
    }
    if (node->type == NodeType::CALL_STATEMENT)
    {
        gen_call(out, node);
        out << ";" << std::endl;
    }
    if (node->type == NodeType::IF_STATEMENT)
    {
        if (node->children.size() == 2)
        {
            out << "if (";
            gen_condition(out, node->children[0]);
            out << ") {" << std::endl;
            gen_statement_list(out, node->children[1]);
            out << "}" << std::endl;
        }
        else
        {
            out << "if (";
            gen_condition(out, node->children[0]);
            out << "){" << std::endl;
            gen_statement_list(out, node->children[1]);
            out << "}else{" << std::endl;
            gen_statement_list(out, node->children[2]);
            out << "}" << std::endl;
        }
    }
    if (node->type == NodeType::WHILE_STATEMENT)
    {
        gen_while(out, node);
    }
}

void ArchCpp::gen_statement_list(std::ofstream &out, Node *node)
{
    if (node->type != NodeType::STATEMENT_LIST)
    {
        gen_statement(out, node);
        return;
    }
    for (auto statement : node->children)
    {
        gen_statement(out, statement);
    }
}

void ArchCpp::gen_block(std::ofstream &out, Node *node)
{
    out << "{" << std::endl;
    gen_statement_list(out, node->children[0]);
    out << "}" << std::endl;
}

void ArchCpp::gen_parameter_list(std::ofstream &out, Node *node)
{
    assert(node->type == NodeType::PARAMETER_LIST || node->type == NodeType::PARAMETER);
    if (node->type == NodeType::PARAMETER)
    {
        gen_type(out, node->children[0]);
        out << " " << node->children[1]->value;
        return;
    }
    for (int i = 0; i < node->children.size(); i++)
    {
        auto child = node->children[i];
        gen_type(out, child->children[0]);
        out << " " << child->children[1]->value;
        if (i != node->children.size() - 1)
        {
            out << ", ";
        }
    }
}

void ArchCpp::gen_function(std::ofstream &out, Node *node)
{
    auto func_name = node->children[1]->value;

    if (func_name == "main")
    {
        out << "int32_t";
    }
    else
    {
        gen_type(out, node->children[0]);
    }

    out << " " << func_name << "(";
    gen_parameter_list(out, node->children[2]);
    out << ")" << std::endl;
    auto block = node->children[3];
    gen_block(out, block);
}