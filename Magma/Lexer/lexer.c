#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

// Fast ASCII helpers
#define IS_ALPHA(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z'))
#define IS_DIGIT(c) ((c) >= '0' && (c) <= '9')
#define IS_ALNUM(c) (IS_ALPHA(c) || IS_DIGIT(c))

// Token type enum
typedef enum {
	// LITERALS
	TOK_IDENTIFIER,
	TOK_NUMBER,
	TOK_FLOAT,
	TOK_STRING,

	// KEYWORDS
	TOK_VAR,
	TOK_CONST,
	TOK_FUNC,
	TOK_RETURN,
	TOK_IF,
	TOK_ELSE,
	TOK_WHILE,
	TOK_BREAK,
	TOK_IMPORT,
	TOK_STRUCT,
	TOK_ENUM,
	TOK_CASE,
	TOK_WAIT,
	TOK_REPEAT,
	TOK_DO,
	TOK_WAITUNTIL,
	TOK_CONTINUE,
	TOK_EXTERN,
	TOK_FOR,
	TOK_GOTO,
	TOK_REGISTER,
	TOK_ARRAY,
	TOK_SWITCH,
	TOK_VOID,
	TOK_START,
	TOK_TYPEDEF,

	// OPERATORS
	TOK_PLUS,
	TOK_MINUS,
	TOK_STAR,
	TOK_SLASH,
	TOK_PERCENT,
	TOK_POWER,

	// EXTRA
	TOK_INC,
	TOK_DEC,
	TOK_QMARK,

	// POINTER
	TOK_POINT_STAR,
	TOK_POINT_AND,

	// ASSIGNMENT
	TOK_ASSIGN,
	TOK_PLUS_ASSIGN,
	TOK_MINUS_ASSIGN,
	TOK_STAR_ASSIGN,
	TOK_SLASH_ASSIGN,
	TOK_PCNT_ASSIGN,

	// COMPARISON
	TOK_EQ_EQ,
	TOK_NOT_EQ,
	TOK_LESSTHAN,
	TOK_GREATERTHAN,
	TOK_LEQ,
	TOK_GEQ,

	// LOGICAL
	TOK_AND,
	TOK_OR,
	TOK_NOT,

	// BITWISE
	TOK_BIT_AND,
	TOK_BIT_OR,
	TOK_BIT_XOR,
	TOK_BIT_NOT,
	TOK_BIT_LSHIFT,
	TOK_BIT_RSHIFT,

	// PUNCTUATION
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_LBRACE,
	TOK_RBRACE,
	TOK_LBRACKET,
	TOK_RBRACKET,
	TOK_COMMA,
	TOK_DOT,
	TOK_COLON,
	TOK_ARROW,
	TOK_BACKSLASH,

	// SPECIAL
	TOK_COMMENT,
	TOK_NEWLINE,
	TOK_EOF
} TokenType;

// Token struct
typedef struct {
	TokenType type;
	const char *start;
	uint32_t length;
} Token;

// Lexer state
typedef struct {
	const char *current;
	const char *end;
} Lexer;

// Initialize lexer
void lexer_init(Lexer *lexer, const char *source, size_t length) {
	lexer->current = source;
	lexer->end = source + length;
}

// Create a token
static inline Token make_token(Lexer *lexer, TokenType type, const char *start) {
	Token currentTOK;
	currentTOK.type = type;
	currentTOK.start = start;
	currentTOK.length = (uint32_t)(lexer->current - start);
	return currentTOK;
}

// Helper to check if string is a keyword
TokenType check_keyword(const char *start, uint32_t length) {

	switch (start[0]) {

	case 'f':
		if (length == 3 && memcmp(start, "for", 3) == 0) return TOK_FOR;
		if (length == 4 && memcmp(start, "func", 4) == 0) return TOK_FUNC;
		break;

	case 'i':
		if (length == 2 && memcmp(start, "if", 2) == 0) return TOK_IF;
		if (length == 6 && memcmp(start, "import", 6) == 0) return TOK_IMPORT;
		break;

	case 'e':
		if (length == 4 && memcmp(start, "else", 4) == 0) return TOK_ELSE;
		if (length == 4 && memcmp(start, "enum", 4) == 0) return TOK_ENUM;
		if (length == 6 && memcmp(start, "extern", 6) == 0) return TOK_EXTERN;
		break;

	case 'w':
		if (length == 5 && memcmp(start, "while", 5) == 0) return TOK_WHILE;
		if (length == 4 && memcmp(start, "wait", 4) == 0) return TOK_WAIT;
		if (length == 9 && memcmp(start, "waituntil", 9) == 0) return TOK_WAITUNTIL;
		break;

	case 'v':
		if (length == 3 && memcmp(start, "var", 3) == 0) return TOK_VAR;
		if (length == 4 && memcmp(start, "void", 4) == 0) return TOK_VOID;
		break;

	case 'c':
		if (length == 5 && memcmp(start, "const", 5) == 0) return TOK_CONST;
		if (length == 4 && memcmp(start, "case", 4) == 0) return TOK_CASE;
		if (length == 8 && memcmp(start, "continue", 8) == 0) return TOK_CONTINUE;
		break;

	case 'r':
		if (length == 6 && memcmp(start, "return", 6) == 0) return TOK_RETURN;
		if (length == 6 && memcmp(start, "repeat", 6) == 0) return TOK_REPEAT;
		if (length == 8 && memcmp(start, "register", 8) == 0) return TOK_REGISTER;
		break;

	case 's':
		if (length == 6 && memcmp(start, "struct", 6) == 0) return TOK_STRUCT;
		if (length == 6 && memcmp(start, "switch", 6) == 0) return TOK_SWITCH;
		if (length == 5 && memcmp(start, "start", 5) == 0) return TOK_START;
		break;

	case 'd':
		if (length == 2 && memcmp(start, "do", 2) == 0) return TOK_DO;
		break;

	case 'g':
		if (length == 4 && memcmp(start, "goto", 4) == 0) return TOK_GOTO;
		break;

	case 'a':
		if (length == 5 && memcmp(start, "array", 5) == 0) return TOK_ARRAY;
		break;

	case 't':
		if (length == 7 && memcmp(start, "typedef", 7) == 0) return TOK_TYPEDEF;
		break;
	}

	return TOK_IDENTIFIER;
}

// Scan identifier or keyword
Token lex_identifier(Lexer *lexer) {

	const char *start = lexer->current;

	while (lexer->current < lexer->end &&
		  (IS_ALNUM(*lexer->current) || *lexer->current == '_'))
		lexer->current++;

	TokenType type = check_keyword(start, (uint32_t)(lexer->current - start));
	return make_token(lexer, type, start);
}

// Scan number (integer or float)
Token lex_number(Lexer *lexer) {

	const char *start = lexer->current;

	while (lexer->current < lexer->end && IS_DIGIT(*lexer->current))
		lexer->current++;

	if (lexer->current < lexer->end && *lexer->current == '.') {

		lexer->current++;

		while (lexer->current < lexer->end && IS_DIGIT(*lexer->current))
			lexer->current++;

		return make_token(lexer, TOK_FLOAT, start);
	}

	return make_token(lexer, TOK_NUMBER, start);
}

// Scan string literal
Token lex_string(Lexer *lexer) {

	const char *start = lexer->current;

	lexer->current++;

	while (lexer->current < lexer->end && *lexer->current != '"')
		lexer->current++;

	if (lexer->current < lexer->end)
		lexer->current++;

	return make_token(lexer, TOK_STRING, start);
}

// Scan comment (single-line)
Token lex_comment(Lexer *lexer) {

	const char *start = lexer->current;

	lexer->current += 2;

	while (lexer->current < lexer->end && *lexer->current != '\n')
		lexer->current++;

	return make_token(lexer, TOK_COMMENT, start);
}

// Skip whitespace
void skip_whitespace(Lexer *lexer) {

	while (lexer->current < lexer->end) {

		char c = *lexer->current;

		if (c == ' ' || c == '\t' || c == '\r')
			lexer->current++;
		else
			break;
	}
}

// Get next token
Token lexer_next(Lexer *lexer) {

	skip_whitespace(lexer);

	if (lexer->current >= lexer->end)
		return make_token(lexer, TOK_EOF, lexer->current);

	const char *start = lexer->current;
	char c = *lexer->current++;

	switch (c) {

	case '+':
		if (*lexer->current == '+') { lexer->current++; return make_token(lexer, TOK_INC, start); }
		if (*lexer->current == '=') { lexer->current++; return make_token(lexer, TOK_PLUS_ASSIGN, start); }
		return make_token(lexer, TOK_PLUS, start);

	case '-':
		if (*lexer->current == '-') { lexer->current++; return make_token(lexer, TOK_DEC, start); }
		if (*lexer->current == '=') { lexer->current++; return make_token(lexer, TOK_MINUS_ASSIGN, start); }
		if (*lexer->current == '>') { lexer->current++; return make_token(lexer, TOK_ARROW, start); }
		return make_token(lexer, TOK_MINUS, start);

	case '*':
		if (*lexer->current == '=') { lexer->current++; return make_token(lexer, TOK_STAR_ASSIGN, start); }
		if (*lexer->current == '*') { lexer->current++; return make_token(lexer, TOK_POWER, start); }
		return make_token(lexer, TOK_STAR, start);

	case '/':
		if (*lexer->current == '/') return lex_comment(lexer);
		if (*lexer->current == '=') { lexer->current++; return make_token(lexer, TOK_SLASH_ASSIGN, start); }
		return make_token(lexer, TOK_SLASH, start);

	case '%':
		if (*lexer->current == '=') { lexer->current++; return make_token(lexer, TOK_PCNT_ASSIGN, start); }
		return make_token(lexer, TOK_PERCENT, start);

	case '^': return make_token(lexer, TOK_BIT_XOR, start);
	case '&': return make_token(lexer, TOK_BIT_AND, start);
	case '|': return make_token(lexer, TOK_BIT_OR, start);
	case '~': return make_token(lexer, TOK_BIT_NOT, start);

	case '!':
		if (*lexer->current == '=') { lexer->current++; return make_token(lexer, TOK_NOT_EQ, start); }
		return make_token(lexer, TOK_NOT, start);

	case '=':
		if (*lexer->current == '=') { lexer->current++; return make_token(lexer, TOK_EQ_EQ, start); }
		return make_token(lexer, TOK_ASSIGN, start);

	case '<':
		if (*lexer->current == '=') { lexer->current++; return make_token(lexer, TOK_LEQ, start); }
		if (*lexer->current == '<') { lexer->current++; return make_token(lexer, TOK_BIT_LSHIFT, start); }
		return make_token(lexer, TOK_LESSTHAN, start);

	case '>':
		if (*lexer->current == '=') { lexer->current++; return make_token(lexer, TOK_GEQ, start); }
		if (*lexer->current == '>') { lexer->current++; return make_token(lexer, TOK_BIT_RSHIFT, start); }
		return make_token(lexer, TOK_GREATERTHAN, start);

	case '?': return make_token(lexer, TOK_QMARK, start);
	case '(': return make_token(lexer, TOK_LPAREN, start);
	case ')': return make_token(lexer, TOK_RPAREN, start);
	case '{': return make_token(lexer, TOK_LBRACE, start);
	case '}': return make_token(lexer, TOK_RBRACE, start);
	case '[': return make_token(lexer, TOK_LBRACKET, start);
	case ']': return make_token(lexer, TOK_RBRACKET, start);
	case ',': return make_token(lexer, TOK_COMMA, start);
	case '.': return make_token(lexer, TOK_DOT, start);
	case ':': return make_token(lexer, TOK_COLON, start);
	case '\\': return make_token(lexer, TOK_BACKSLASH, start);
	case '\n': return make_token(lexer, TOK_NEWLINE, start);
	case '"': return lex_string(lexer);

	default:
		if (IS_ALPHA(c) || c == '_') return lex_identifier(lexer);
		if (IS_DIGIT(c)) return lex_number(lexer);
		break;
	}

	return make_token(lexer, TOK_IDENTIFIER, start);
}