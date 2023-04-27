#pragma once

#include <vector>
#include <memory>
#include <string>

#include "nodetypes.hpp"

struct Node
{
    NodeType type = NodeType::PROGRAM;
    std::vector<Node *> children = {};
    std::string value = "";
    int line = 0;
};

Node *make_node(NodeType type, const std::vector<Node *> &children, const std::string &value);

int store_node(Node *node);
Node *get_node(int index);

void print_node(Node *node, int indent = 0);

Node *simplify_tree(Node *node);

Node *flatten_globals(Node *node);

void check_types(Node *node);