#pragma once

#include <string>
#include <vector>

struct Tac {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

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
