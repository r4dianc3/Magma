#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "declaration.c"
#include "statement.c"
#include "expression.c"

typedef struct ParserStateStruct
{
    Tokenstream *ts;

    uint32_t current;

    int had_error;
    int panic_mode;
} Parser;

// Return current token without consuming it
Token peek(Parser *p)
{
    return p->tokens[p->current];
}

// Return previously consumed token
Token previous(Parser *p)
{
    return p->tokens[p->current - 1];
}

// Check if we've reached EOF
int is_at_end(Parser *p)
{
    return peek(p).type == TOK_EOF;
}

// Move to next token
Token advance(Parser *p)
{
    if (!is_at_end(p))
        p->current++;
    return previous(p);
}

// Check current token type without consuming
int check(Parser *p, TokenType type)
{
    if (is_at_end(p))
        return 0;
    return peek(p).type == type;
}

// If matches, consume it
int match(Parser *p, TokenType type)
{
    if (check(p, type))
    {
        advance(p);
        return 1;
    }
    return 0;
}

// Consume required token or error
Token expect(Parser *p, TokenType type, const char *message)
{
    if (check(p, type))
        return advance(p);

    // error handling (minimal)
    printf("Parser error: %s at line %d\n", message, peek(p).line);

    // return dummy token to avoid crash
    return peek(p);
}

// Error
void parser_error(Parser *p, const char *message)
{
    parser_error_at(p, peek(p), message);
}

// Where error
void parser_error_at(Parser *p, Token token, const char *message)
{
    if (p->panic_mode)
        return;
    p->panic_mode = 1;
    p->had_error = 1;

    fprintf(stderr, "[line %d] Error", token.line);

    if (token.type == TOK_EOF)
    {
        fprintf(stderr, " at end");
    }
    else
    {
        fprintf(stderr, " at '%.*s'", token.length, token.start);
    }

    fprintf(stderr, ": %s\n", message);
}

// Recovery
void synchronize(Parser *p)
{
    p->panic_mode = 0;

    while (!is_at_end(p))
    {

        if (previous(p).type == TOK_SEMICOLON)
            return;

        switch (peek(p).type)
        {
        case TOK_FUNC:
        case TOK_VAR:
        case TOK_IF:
        case TOK_FOR:
        case TOK_WHILE:
        case TOK_RETURN:
            return;
        default:
            break;
        }

        advance(p);
    }
}