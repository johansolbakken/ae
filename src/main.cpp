#include <iostream>
#include <vector>
#include <fstream>

#include "tree.hpp"
#include "generator.hpp"
#include "preprocess.hpp"
#include "arch/arch_cpp.hpp"
#include "arch/arch_js.hpp"

Node *root = nullptr;

constexpr auto panic = 0;

int yyparse();
extern FILE *yyin;

struct Options
{
    bool parse_tree = false;
    bool keep_cpp = false;
    bool keep_out_ae = false;
    std::string output_file = "a.out";
    bool javascript = false;
    std::vector<std::string> source_files;
};

Options extractOptions(int argc, char **argv)
{
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
        else if (std::string(argv[i]) == "--javascript")
        {
            options.javascript = true;
        }
        else
        {
            options.source_files.push_back(argv[i]);
        }
    }

    return options;
}

int main(int argc, char **argv)
{
    Options options = extractOptions(argc, argv);

    if (panic)
    {
        options.keep_cpp = true;
        options.keep_out_ae = true;
        options.parse_tree = true;
    }

    if (options.source_files.size() == 0)
    {
        std::cout << "No source files provided" << std::endl;
        return 1;
    }

    if (options.output_file == "a.out")
    {
        // set output file to source file name without extension and without path
        options.output_file = options.source_files[0];
        int last_slash = options.output_file.find_last_of('/');
        if (last_slash != std::string::npos)
            options.output_file = options.output_file.substr(last_slash + 1);
        int last_dot = options.output_file.find_last_of('.');
        if (last_dot != std::string::npos)
            options.output_file = options.output_file.substr(0, last_dot);
    }

    if (options.javascript)
    {
        options.output_file += ".js";
    }

    preprocess(options.source_files[0], "out.ae");

    yyin = fopen("out.ae", "r");

    // Parsing
    yyparse();
    root = flatten_globals(root);
    root = simplify_tree(root);
    if (options.parse_tree)
        print_node(root);

    // Analysis
    check_types(root);

    // Generate code
    if (options.javascript)
    {
        generate_code(root, options.output_file, std::make_shared<ArchJs>());
    }
    else
    {
        generate_code(root, "out.cpp", std::make_shared<ArchCpp>());
        compile_code("out.cpp", options.output_file);
    }
    if (!options.keep_cpp)
        system("rm out.cpp");
    if (!options.keep_out_ae)
        system("rm out.ae");

    return 0;
}