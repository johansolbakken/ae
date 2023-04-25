#include <iostream>

#include "tree.hpp"
#include "generator.hpp"

Node* root = nullptr;

int yyparse();

int main() {
    // Parsing
    yyparse();
    root = simplify_tree(root);
    print_node(root);

    // Generate code
    generate_code(root, "out.cpp");
    compile_code("out.cpp", "program");
    system("rm out.cpp");

    return 0;
}