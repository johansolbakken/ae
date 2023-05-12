#pragma once

#include <string>
#include <vector>

#include "tree.hpp"

struct Tac {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

struct Ir {
    std::vector<Tac> tacs;
    std::vector<std::string> strings;
    std::vector<std::string> globals;
    std::vector<std::string> functions;
    std::vector<std::string> externs;
};

Ir generateIr(Node *root);


//  x = y op z
//  x = op y
//  x = y
//  goto L
//  if x goto L
//  ifFalse x goto L
//  if x relop y goto L
//  param x
//  call x, n
//  x = call y, n
//  return x
//  x = y[i]
//  x[i] = y
//  x = &y
//  x = *y
//  *x = y
