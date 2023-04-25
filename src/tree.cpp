#include "tree.hpp"

#include <unordered_map>
#include <iostream>

Node *make_node(NodeType type, const std::vector<Node *> &children, const std::string &value)
{
    Node *node = new Node();
    node->type = type;
    node->children = children;
    node->value = value;
    return node;
}

static std::unordered_map<int, Node *> nodes = {};

int store_node(Node *node)
{
    static int index = 0;
    nodes[index] = node;
    return index++;
}

Node *get_node(int index)
{
    return nodes[index];
}

std::string node_type_to_string(NodeType type)
{
    switch (type)
    {
    case NodeType::PROGRAM:
        return "PROGRAM";
    case NodeType::GLOBAL_LIST:
        return "GLOBAL_LIST";
    case NodeType::GLOBAL:
        return "GLOBAL";
    case NodeType::STATEMENT:
        return "STATEMENT";
    case NodeType::BLOCK:
        return "BLOCK";
    case NodeType::STATEMENT_LIST:
        return "STATEMENT_LIST";
    case NodeType::RETURN_STATEMENT:
        return "RETURN_STATEMENT";
    case NodeType::PRINT_STATEMENT:
        return "PRINT_STATEMENT";
    case NodeType::IDENTIFIER_DATA:
        return "IDENTIFIER_DATA";
    case NodeType::INT_DATA:
        return "INT_DATA";
    case NodeType::STRING_DATA:
        return "STRING_DATA";
    case NodeType::TYPE_INT:
        return "TYPE_INT";
    case NodeType::FUNCTION:
        return "FUNCTION";
    default:
        return "UNKNOWN";
    }
}

void print_node(Node *node, int indent)
{
    for (int i = 0; i < indent; i++)
    {
        std::cout << "  ";
    }
    std::cout << node_type_to_string(node->type);
    if (node->type == NodeType::INT_DATA || node->type == NodeType::STRING_DATA || node->type == NodeType::IDENTIFIER_DATA)
    {
        std::cout << " (" << node->value << ")";
    }
    std::cout << std::endl;
    for (Node *child : node->children)
    {
        print_node(child, indent + 1);
    }
}

Node* simplify_tree(Node *node)
{
    for (int i = 0; i < node->children.size(); i++) {
        auto* child = node->children[i];
        node->children[i] = simplify_tree(child);
    }

    if (node->type == NodeType::PROGRAM) {
        auto* child = node->children[0];
        delete node;
        return child;
    }

    if (node->type == NodeType::STATEMENT) {
        auto* child = node->children[0];
        delete node;
        return child;
    }

    if (node->type == NodeType::STATEMENT_LIST) {
        if (node->children.size() == 1) {
            auto* child = node->children[0];
            delete node;
            return child;
        }
        if (node->children[0]->type == NodeType::STATEMENT_LIST) {
            auto* child = node->children[0];
            child->children.push_back(node->children[1]);
            delete node;
            return child;
        }
    }

    return node;
}