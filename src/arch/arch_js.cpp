#include "arch/arch_js.hpp"
#include "utils.hpp"

#include <iostream>

void ArchJs::gen_header(std::ofstream &out, Node *node)
{
}
void ArchJs::gen_footer(std::ofstream &out, Node *node)
{
    out << "main()\n";
}

void ArchJs::gen_program(std::ofstream &out, Node *node)
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
void ArchJs::gen_type(std::ofstream &out, Node *node)
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

void ArchJs::gen_condition(std::ofstream &out, Node *node)
{
}
void ArchJs::gen_while(std::ofstream &out, Node *node)
{
}
void ArchJs::gen_call(std::ofstream &out, Node *node)
{
    out << node->children[0]->value << "(";
    gen_expression_list(out, node->children[0]);
    out << ")";
}
void ArchJs::gen_array_indexing(std::ofstream &out, Node *node)
{
    out << node->children[0]->value << "[";
    gen_expression(out, node->children[1]);
    out << "]";
}
void ArchJs::gen_variable_declaration(std::ofstream &out, Node *node)
{
    if (node->children[0]->type == NodeType::TYPE_POINTER)
    {
        out << "let " << node->children[1]->value << " = ";
        gen_expression(out, node->children[2]);
        out << ";" << std::endl;
        out << "let ptr_" << node->children[1]->value << " = 0;" << std::endl;
    }
    else
    {
        out << "let " << node->children[1]->value << " = ";
        gen_expression(out, node->children[2]);
        out << ";" << std::endl;
    }
}
void ArchJs::gen_array_declaration(std::ofstream &out, Node *node)
{
    auto type = node->children[0]->type;
    auto identifier = node->children[2]->value;

    out << "let " << identifier << " = [];" << std::endl;
    out << "for (let i = 0; i < ";
    gen_expression(out, node->children[1]);
    out << "; i++) {" << std::endl;
    out << identifier << ".push(";
    if (is_floating_type(type) || is_integer_type(type))
    {
        out << "0";
    }
    else if (is_boolean_type(type))
    {
        out << "false";
    }
    else
    {
        std::cout << "Unknown type: " << node_type_to_string(type) << " at line " << node->line << std::endl;
        exit(1);
    }
    out << ");" << std::endl;
    out << "}\n";
}

void ArchJs::gen_expression_list(std::ofstream &out, Node *node)
{
}
void ArchJs::gen_expression(std::ofstream &out, Node *node)
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
        if (node->value == "-")
        {
            out << "-";
        }

        gen_expression(out, node->children[0]);

        if (node->value == "*")
        {
            out << "[";
            out << "ptr_" << node->children[0]->children[0]->value;
            out << "]";
        }
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
void ArchJs::gen_statement(std::ofstream &out, Node *node)
{
    if (node->type == NodeType::PRINT_STATEMENT)
    {
        out << "console.log(";
        gen_expression(out, node->children[0]);
        out << ");";
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
            gen_expression(out, node->children[0]);
        }
        else if (node->children[0]->type == NodeType::ARRAY_INDEXING)
        {
            gen_array_indexing(out, node->children[0]);
        }
        else
        {
            // todo: copy array or something?
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
void ArchJs::gen_statement_list(std::ofstream &out, Node *node)
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
void ArchJs::gen_block(std::ofstream &out, Node *node)
{
    out << "{" << std::endl;
    gen_statement_list(out, node->children[0]);
    out << "}" << std::endl;
}
void ArchJs::gen_parameter_list(std::ofstream &out, Node *node)
{
}
void ArchJs::gen_function(std::ofstream &out, Node *node)
{
    out << "const ";

    auto func_name = node->children[1]->value;
    out << func_name << " = (";

    gen_parameter_list(out, node->children[2]);

    out << ") => ";

    auto block = node->children[3];
    gen_block(out, block);
}