#pragma once

#include "tree.hpp"

#include <fstream>

class Arch
{
public:
    virtual ~Arch() = default;
    virtual void gen_header(std::ofstream &out, Node *node) = 0;
    virtual void gen_footer(std::ofstream &out, Node *node) = 0;
    virtual void gen_program(std::ofstream &out, Node *node) = 0;
    virtual void gen_type(std::ofstream &out, Node *node) = 0;
    virtual void gen_condition(std::ofstream &out, Node *node) = 0;
    virtual void gen_while(std::ofstream &out, Node *node) = 0;
    virtual void gen_call(std::ofstream &out, Node *node) = 0;
    virtual void gen_array_indexing(std::ofstream &out, Node *node) = 0;
    virtual void gen_variable_declaration(std::ofstream &out, Node *node) = 0;
    virtual void gen_array_declaration(std::ofstream &out, Node *node) = 0;
    virtual void gen_expression_list(std::ofstream &out, Node *node) = 0;
    virtual void gen_expression(std::ofstream &out, Node *node) = 0;
    virtual void gen_statement(std::ofstream &out, Node *node) = 0;
    virtual void gen_statement_list(std::ofstream &out, Node *node) = 0;
    virtual void gen_block(std::ofstream &out, Node *node) = 0;
    virtual void gen_parameter_list(std::ofstream &out, Node *node) = 0;
    virtual void gen_function(std::ofstream &out, Node *node) = 0;
};
