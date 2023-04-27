#include "utils.hpp"

bool contains_string(const std::vector<std::string>& strings, const std::string& string)
{
    for (const auto& string1 : strings)
    {
        if (string1 == string) {
            return true;
        } 
    }
    return false;
}