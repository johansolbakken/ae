#include <iostream>
#include <vector>
#include <fstream>

#include "tree.hpp"
#include "generator.hpp"
#include "preprocess.hpp"

Node *root = nullptr;

int yyparse();
extern FILE *yyin;

struct Options
{
    bool parse_tree = true;
    bool keep_cpp = true;
    bool keep_out_ae = false;
    std::string output_file = "a.out";
};

int main(int argc, char **argv)
{
    std::vector<std::string> source_files;

    Options options;
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "--parse-tree")
        {
            options.parse_tree = true;
        }
        else if (std::string(argv[i]) == "--keep-cpp")
        {
            options.keep_cpp = true;
        }
        else if (std::string(argv[i]) == "--output" || std::string(argv[i]) == "-o")
        {
            options.output_file = argv[i + 1];
            i++;
        }
        else
        {
            source_files.push_back(argv[i]);
        }
    }

    if (source_files.size() == 0)
    {
        std::cout << "No source files provided" << std::endl;
        return 1;
    }

    if (options.output_file == "a.out")
    {
        // set output file to source file name without extension and without path
        options.output_file = source_files[0];
        int last_slash = options.output_file.find_last_of('/');
        if (last_slash != std::string::npos)
            options.output_file = options.output_file.substr(last_slash + 1);
        int last_dot = options.output_file.find_last_of('.');
        if (last_dot != std::string::npos)
            options.output_file = options.output_file.substr(0, last_dot);
    }

    preprocess(source_files[0], "out.ae");

    yyin = fopen("out.ae", "r");

    // Parsing
    yyparse();
    root = flatten_globals(root);
    root = simplify_tree(root);
    if (options.parse_tree)
        print_node(root);

    // Generate code
    generate_code(root, "out.cpp");
    compile_code("out.cpp", options.output_file);
    if (!options.keep_cpp)
        system("rm out.cpp");
    if (!options.keep_out_ae)
        system("rm out.ae");

    return 0;
}