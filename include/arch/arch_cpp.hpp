#pragma once

#include "arch/arch.hpp"

class ArchCpp : public Arch
{
public:
    ArchCpp() = default;
    ~ArchCpp() override = default;
    void gen_header(std::ofstream &out, Node *node) override;
    void gen_footer(std::ofstream &out, Node *node) override;
    void gen_program(std::ofstream&out, Node* node) override;
    void gen_type(std::ofstream &out, Node *node) override;
    void gen_condition(std::ofstream &out, Node *node) override;
    void gen_while(std::ofstream &out, Node *node) override;
    void gen_call(std::ofstream &out, Node *node) override;
    void gen_array_indexing(std::ofstream &out, Node *node) override;
    void gen_variable_declaration(std::ofstream &out, Node *node) override;
    void gen_array_declaration(std::ofstream &out, Node *node) override;
    void gen_expression_list(std::ofstream &out, Node *node) override;
    void gen_expression(std::ofstream &out, Node *node) override;
    void gen_statement(std::ofstream &out, Node *node) override;
    void gen_statement_list(std::ofstream &out, Node *node) override;
    void gen_block(std::ofstream &out, Node *node) override;
    void gen_parameter_list(std::ofstream &out, Node *node) override;
    void gen_function(std::ofstream &out, Node *node) override;
};
