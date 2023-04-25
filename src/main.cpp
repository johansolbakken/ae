#include <iostream>

#include "tree.hpp"

Node* root = nullptr;

int yyparse();

int main() {
    yyparse();
    print_node(root);
    root = simplify_tree(root);
    print_node(root);

    return 0;
}