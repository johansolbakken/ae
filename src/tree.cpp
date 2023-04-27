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
    case NodeType::FUNCTION:
        return "FUNCTION";
    case NodeType::DECLARATION:
        return "DECLARATION";
    case NodeType::EXPRESSION:
        return "EXPRESSION";
    case NodeType::ASSIGNMENT_STATEMENT:
        return "ASSIGNMENT_STATEMENT";
    case NodeType::TYPE_VOID:
        return "TYPE_VOID";
    case NodeType::CALL_STATEMENT:
        return "CALL_STATEMENT";
    case NodeType::IF_STATEMENT:
        return "IF_STATEMENT";
    case NodeType::CONDITION:
        return "CONDITION";
    case NodeType::WHILE_STATEMENT:
        return "WHILE_STATEMENT";
    case NodeType::FLOAT_DATA:
        return "FLOAT_DATA";
    case NodeType::POINTER:
        return "POINTER";
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
    if (node->value != "")
        std::cout << " (" << node->value << ")";
    std::cout << std::endl;
    for (Node *child : node->children)
    {
        print_node(child, indent + 1);
    }
}

Node *flatten_globals(Node *node)
{
    for (int i = 0; i < node->children.size(); i++)
    {
        auto *child = node->children[i];
        node->children[i] = flatten_globals(child);
    }

    if (node->type == NodeType::GLOBAL_LIST)
    {
        if (node->children.size() == 1)
        {
            auto *child = node->children[0];
            delete node;
            return child;
        }
        if (node->children[0]->type == NodeType::GLOBAL_LIST)
        {
            auto *child = node->children[0];
            child->children.push_back(node->children[1]);
            delete node;
            return child;
        }
    }

    if (node->type == NodeType::GLOBAL)
    {
        if (node->children.size() == 1)
        {
            auto *child = node->children[0];
            delete node;
            return child;
        }
    }

    return node;
}

Node *simplify_tree(Node *node)
{
    for (int i = 0; i < node->children.size(); i++)
    {
        auto *child = node->children[i];
        node->children[i] = simplify_tree(child);
    }

    if (node->type == NodeType::PROGRAM)
    {
        auto *child = node->children[0];
        delete node;
        return child;
    }

    if (node->type == NodeType::STATEMENT)
    {
        auto *child = node->children[0];
        delete node;
        return child;
    }

    if (node->type == NodeType::STATEMENT_LIST)
    {
        if (node->children.size() == 1)
        {
            auto *child = node->children[0];
            delete node;
            return child;
        }
        if (node->children[0]->type == NodeType::STATEMENT_LIST)
        {
            auto *child = node->children[0];
            child->children.push_back(node->children[1]);
            delete node;
            return child;
        }
    }

    if (node->type == NodeType::EXPRESSION)
    {
        // Constant folding
        if (node->children.size() == 2 &&
            node->children[0]->type == NodeType::INT_DATA &&
            node->children[1]->type == NodeType::INT_DATA)
        {
            if (node->value == "+")
            {
                auto *child = node->children[0];
                child->value = std::to_string(std::stoi(child->value) + std::stoi(node->children[1]->value));
                delete node->children[1];
                delete node;
                return child;
            }
            if (node->value == "-")
            {
                auto *child = node->children[0];
                child->value = std::to_string(std::stoi(child->value) - std::stoi(node->children[1]->value));
                delete node->children[1];
                delete node;
                return child;
            }
            if (node->value == "*")
            {
                auto *child = node->children[0];
                child->value = std::to_string(std::stoi(child->value) * std::stoi(node->children[1]->value));
                delete node->children[1];
                delete node;
                return child;
            }
            if (node->value == "/")
            {
                auto *child = node->children[0];
                child->value = std::to_string(std::stoi(child->value) / std::stoi(node->children[1]->value));
                delete node->children[1];
                delete node;
                return child;
            }
        }
    }

    return node;
}

bool is_integer_type(NodeType type)
{
    switch (type)
    {
    case NodeType::TYPE_I8:
    case NodeType::TYPE_I16:
    case NodeType::TYPE_I32:
    case NodeType::TYPE_I64:
    case NodeType::TYPE_U8:
    case NodeType::TYPE_U16:
    case NodeType::TYPE_U32:
    case NodeType::TYPE_U64:
        return true;
    default:
        return false;
    }
}

bool is_floating_type(NodeType type)
{
    switch (type)
    {
    case NodeType::TYPE_F32:
    case NodeType::TYPE_F64:
        return true;
    default:
        return false;
    }
}

void check_types(Node *node)
{
    for (int i = 0; i < node->children.size(); i++)
    {
        auto *child = node->children[i];
        check_types(child);
    }

    if (node->type == NodeType::DECLARATION)
    {
        if (is_integer_type(node->children[0]->type))
        {
            if (node->children[1]->type != NodeType::INT_DATA)
            {
                std::cout << "ERROR line " << node->line << ": Expected int data" << std::endl;
                exit(1);
            }
        }
        if (is_floating_type(node->children[0]->type))
        {
            if (node->children[1]->type != NodeType::FLOAT_DATA)
            {
                std::cout << "ERROR line " << node->line << ": Expected float data" << std::endl;
                exit(1);
            }
        }
    }
}