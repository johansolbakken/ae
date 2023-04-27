#pragma once

enum class NodeType
{
    PROGRAM,
    GLOBAL_LIST,
    GLOBAL,
    STATEMENT,
    BLOCK,
    STATEMENT_LIST,
    RETURN_STATEMENT,
    PRINT_STATEMENT,
    PARAMETER_LIST,
    PARAMETER,
    EXPRESSION_LIST,
    ARRAY_INDEXING,
    INITIALIZER,
    ARRAY_DECLARATION,
    VARIABLE_DECLARATION,
    
    IDENTIFIER_DATA,
    INT_DATA,
    STRING_DATA,
    FLOAT_DATA,

    TYPE,
    TYPE_VOID,
    TYPE_BOOL,
    TYPE_I8,
    TYPE_I16,
    TYPE_I32,
    TYPE_I64,
    TYPE_U8,
    TYPE_U16,
    TYPE_U32,
    TYPE_U64,
    TYPE_F32,
    TYPE_F64,
    TYPE_POINTER,
    
    FUNCTION,
    DECLARATION,
    EXPRESSION,
    ASSIGNMENT_STATEMENT,
    CALL_STATEMENT,
    IF_STATEMENT,
    CONDITION,
    WHILE_STATEMENT,
    INSERT_STATEMENT,
    POINTER
};