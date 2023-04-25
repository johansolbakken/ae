#include <iostream>

#include "tree.hpp"
#include "generator.hpp"

Node *root = nullptr;

int yyparse();

struct Options
{
    bool parse_tree = false;
    bool keep_cpp = false;
};

int main(int argc, char **argv)
{
    Options options;
    for (int i = 0; i < argc; i++)
    {
        if (std::string(argv[i]) == "--parse-tree")
        {
            options.parse_tree = true;
        }
        else if (std::string(argv[i]) == "--keep-cpp")
        {
            options.keep_cpp = true;
        }
    }

    // Parsing
    yyparse();
    root = simplify_tree(root);
    if (options.parse_tree)
        print_node(root);

    // Generate code
    generate_code(root, "out.cpp");
    compile_code("out.cpp", "program");
    if (!options.keep_cpp)
        system("rm out.cpp");

    return 0;
}