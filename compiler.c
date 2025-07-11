#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.c"

Token tok;

void match(TokenType expected) {
    if (tok.type == expected)
        tok = next_token();
    else {
        printf("Unexpected token: %s\n", tok.lexeme);
        exit(1);
    }
}

void expr(FILE *out) {
    if (tok.type == TOKEN_INT || tok.type == TOKEN_ID) {
        fprintf(out, "%s", tok.lexeme);
        tok = next_token();
        if (tok.type == TOKEN_PLUS) {
            fprintf(out, " + ");
            match(TOKEN_PLUS);
            expr(out);
        }
        else if (tok.type == TOKEN_MINUS) {
          fprintf(out, "-");
          match(TOKEN_MINUS);
          expr(out);
        }
    } else {
        printf("Syntax error in expression: %s\n", tok.lexeme);
        exit(1);
    }
}

void stmt(FILE *out) {
    if (tok.type == TOKEN_ID) {
        char varname[64];
        strcpy(varname, tok.lexeme);
        match(TOKEN_ID);
        match(TOKEN_ASSIGN);
        fprintf(out, "int %s = ", varname);
        expr(out);
        fprintf(out, ";\n");
    } else if (tok.type == TOKEN_PRINT) {
        match(TOKEN_PRINT);
        match(TOKEN_LPAREN);
        fprintf(out, "printf(\"%%d\\n\", ");
        expr(out);
        fprintf(out, ");\n");
        match(TOKEN_RPAREN);
    } else {
        printf("Unknown statement: %s\n", tok.lexeme);
        exit(1);
    }
}

void program(FILE *out) {
    tok = next_token();
    while (tok.type != TOKEN_EOF) {
        if (tok.type == TOKEN_EOL) {
            tok = next_token();
            continue;
        }
        stmt(out);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    source = fopen(argv[1], "r");
    if (!source) {
        printf("Cannot open source file.\n");
        return 1;
    }

    read_char();

    FILE *out = fopen("output.c", "w");
    fprintf(out, "#include <stdio.h>\n\nint main() {\n");

    program(out);

    fprintf(out, "return 0;\n}\n");
    fclose(out);
    fclose(source);

    printf("Compiled to output.c\n");
    return 0;
}

