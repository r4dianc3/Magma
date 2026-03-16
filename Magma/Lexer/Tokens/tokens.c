#include "tokens.h"

// Operator list makes it so that all the operators' info is easy to access
Operator operators[] = {
    {"+=", 2, TOK_PLUS_ASSIGN},
    {"++", 2, TOK_INC},
    {"+", 1, TOK_PLUS},

    {"-=", 2, TOK_MINUS_ASSIGN},
    {"--", 2, TOK_DEC},
    {"->", 2, TOK_ARROW},
    {"-", 1, TOK_MINUS},

    {"*=", 2, TOK_STAR_ASSIGN},
    {"**", 2, TOK_POWER},
    {"*", 1, TOK_STAR},

    {"/=", 2, TOK_SLASH_ASSIGN},
    {"/", 1, TOK_SLASH},

    {"%=", 2, TOK_PCNT_ASSIGN},
    {"%", 1, TOK_PERCENT},

    {"==", 2, TOK_EQ_EQ},
    {"=", 1, TOK_ASSIGN},

    {"!=", 2, TOK_NOT_EQ},
    {"!", 1, TOK_NOT},

    {"<=", 2, TOK_LEQ},
    {"<<", 2, TOK_BIT_LSHIFT},
    {"<", 1, TOK_LESSTHAN},

    {">=", 2, TOK_GEQ},
    {">>", 2, TOK_BIT_RSHIFT},
    {">", 1, TOK_GREATERTHAN},

    {"&&", 2, TOK_AND},
    {"||", 2, TOK_OR},

    {"&", 1, TOK_BIT_AND},
    {"|", 1, TOK_BIT_OR},
    {"^", 1, TOK_BIT_XOR},
    {"~", 1, TOK_BIT_NOT},

    {"?", 1, TOK_QMARK},

    {NULL, 0, 0}};

// Construct a Token given the lexer, type, and starting position.
// The length is computed from the difference between the current pointer
// and the start pointer.
Token make_token(TokenType type, const char *start, uint32_t length, uint32_t line, uint32_t column)
{
    Token token;
    token.type = type;
    token.start = start;
    token.length = length;
    token.line = line;
    token.column = column;
    return token;
}