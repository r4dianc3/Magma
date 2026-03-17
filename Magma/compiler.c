#include "Lexer/lexer.c"
#include "Parser/parser.c"
#include "semantic/symbol_table.c"
#include "semantic/type_checker.c"
#include "ast/ast.c"

void init() {
    void lexer_init();
    void tokenstream_init();
    void tokenstream_free();
}

int main()
{
    void lexer_scan_all();

    return 0;
}