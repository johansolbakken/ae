#include "preprocess.hpp"

#include "utils.hpp"

#include <iostream>
#include <fstream>
#include <vector>

void preprocess(const std::string &filename, const std::string &output_filename)
{
    std::vector<std::string> included_files = {filename};

    // find path of file
    std::string path = filename.substr(0, filename.find_last_of('/'));

    // read whole filename into a string
    std::ifstream file(filename);

    // for each file check if it starts with insert and if so, read that file and insert it
    // at the position of the insert statement
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
    {
        if (line.find("#insert") == 0)
        {
            std::string insert_filename = path + "/" + line.substr(8) + ".ae";
            if (contains_string(included_files, insert_filename))
            {
                continue;
            }
            std::ifstream insert_file = std::ifstream(insert_filename);
            if (!insert_file.is_open())
            {
                std::cout << "Could not open file " << insert_filename << std::endl;
                return;
            }
            std::string insert_line;
            while (std::getline(insert_file, insert_line))
            {
                lines.push_back(insert_line);
            }
            insert_file.close();
            included_files.push_back(insert_filename);
        }
        else
        {
            lines.push_back(line);
        }
    }

    file.close();

    std::string output = "";
    for (auto &line : lines)
    {
        output += line + "\n";
    }

    std::ofstream output_file(output_filename);
    output_file << output;
    output_file.close();
}