#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// TokenTypeEnum defines all possible token kinds the lexer can produce.
typedef enum TokenTypeEnum
{
    // LITERALS: identifiers, numbers, strings
    TOK_IDENTIFIER,
    TOK_NUMBER,
    TOK_FLOAT,
    TOK_STRING,

    // KEYWORDS: reserved words with special meaning
    TOK_VAR,   // VARIABLE
    TOK_CONST, // CONSTANT
    TOK_FUNC,  // FUNCTION
    TOK_RETURN,
    TOK_IF,
    TOK_ELSE,
    TOK_WHILE,
    TOK_BREAK, // BREAK LOOP
    TOK_IMPORT,
    TOK_STRUCT,
    TOK_ENUM, // ENUMERATION
    TOK_CASE,
    TOK_WAIT,
    TOK_REPEAT,
    TOK_DO, // DO-WHILE loop
    TOK_WAITUNTIL,
    TOK_CONTINUE,
    TOK_EXTERN, // EXTERNAL FUNCTION
    TOK_FOR,
    TOK_GOTO,
    TOK_REGISTER,
    TOK_ARRAY,
    TOK_SWITCH,
    TOK_VOID,
    TOK_START,
    TOK_TYPEDEF,
    TOK_GLOBAL,
    TOK_PRINTT, // PRINT TO CONSOLE
    TOK_LIB,    // IMPORT LIBRARY

    // OPERATORS: arithmetic and other symbols
    TOK_PLUS,    // +
    TOK_MINUS,   // -
    TOK_STAR,    // *
    TOK_SLASH,   // /
    TOK_PERCENT, // %
    TOK_POWER,   // **

    // EXTRA: increment, decrement, question mark
    TOK_INC,   // ++
    TOK_DEC,   // --
    TOK_QMARK, // ?

    // POINTER: pointer operators
    TOK_POINT_STAR, // @
    TOK_POINT_AND,  // `

    // ASSIGNMENT: various compound assignment operators
    TOK_ASSIGN,       // =
    TOK_PLUS_ASSIGN,  // +=
    TOK_MINUS_ASSIGN, // -=
    TOK_STAR_ASSIGN,  // *=
    TOK_SLASH_ASSIGN, // /=
    TOK_PCNT_ASSIGN,  // %=

    /* COMPARISON operators */
    TOK_EQ_EQ,       // ==
    TOK_NOT_EQ,      // !=
    TOK_LESSTHAN,    // <
    TOK_GREATERTHAN, // >
    TOK_LEQ,         // <=
    TOK_GEQ,         // >=

    /* LOGICAL operators */
    TOK_AND, // &&
    TOK_OR,  // ||
    TOK_NOT, // !

    /* BITWISE operators */
    TOK_BIT_AND,    // &
    TOK_BIT_OR,     // |
    TOK_BIT_XOR,    // ^
    TOK_BIT_NOT,    // ~
    TOK_BIT_LSHIFT, // <<
    TOK_BIT_RSHIFT, // >>

    /* PUNCTUATION */
    TOK_LPAREN,    // (
    TOK_RPAREN,    // )
    TOK_LBRACE,    // {
    TOK_RBRACE,    // }
    TOK_LBRACKET,  // [
    TOK_RBRACKET,  // ]
    TOK_COMMA,     // ,
    TOK_DOT,       // .
    TOK_COLON,     // :
    TOK_ARROW,     // ->
    TOK_BACKSLASH, // \

    /* SPECIAL tokens */
    TOK_COMMENT, // //
    TOK_NEWLINE, // \n
    TOK_EOF      // End of file
} TokenType;

typedef struct OperatorStruct
{
    const char *text;
    uint8_t length;
    TokenType type;
} Operator;

// Operator list makes it so that all the operators' info is easy to access
static Operator operators[] = {
    {"+=", 2, TOK_PLUS_ASSIGN},
    {"++", 2, TOK_INC},
    {"+", 1, TOK_PLUS},

    {"-=", 2, TOK_MINUS_ASSIGN},
    {"--", 2, TOK_DEC},
    {"->", 2, TOK_ARROW},
    {"-", 1, TOK_MINUS},

    {"*=", 2, TOK_STAR_ASSIGN},
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

// Representation of a single token produced by the lexer
// `type` indicates the kind of token, `start` points into the source
// buffer at the first character of the token, and `length` stores how many
// characters long the token is.
typedef struct TokenStruct
{
    TokenType type;
    const char *start;
    uint32_t length;
} Token;