#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef enum {
    TOKEN_INT, TOKEN_ID, TOKEN_ASSIGN,
    TOKEN_PLUS, TOKEN_PRINT, TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_EOL, TOKEN_EOF, TOKEN_UNKNOWN, TOKEN_MINUS
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[64];
} Token;

FILE *source;
int ch;

void read_char() { ch = fgetc(source); }

void skip_whitespace() {
    while (ch == ' ' || ch == '\t') read_char();
}

Token next_token() {
    Token tok;
    skip_whitespace();

    if (isdigit(ch)) {
        int i = 0;
        do {
            tok.lexeme[i++] = ch;
            read_char();
        } while (isdigit(ch));
        tok.lexeme[i] = '\0';
        tok.type = TOKEN_INT;
    } else if (isalpha(ch)) {
        int i = 0;
        do {
            tok.lexeme[i++] = ch;
            read_char();
        } while (isalnum(ch));
        tok.lexeme[i] = '\0';
        if (strcmp(tok.lexeme, "print") == 0)
            tok.type = TOKEN_PRINT;
        else
            tok.type = TOKEN_ID;
    } else {
        switch (ch) {
            case '=': tok.type = TOKEN_ASSIGN; strcpy(tok.lexeme, "="); break;
            case '+': tok.type = TOKEN_PLUS; strcpy(tok.lexeme, "+"); break;
            case '-': tok.type = TOKEN_MINUS; strcpy(tok.lexeme, "-"); break;
            case '(': tok.type = TOKEN_LPAREN; strcpy(tok.lexeme, "("); break;
            case ')': tok.type = TOKEN_RPAREN; strcpy(tok.lexeme, ")"); break;
            case '\n': tok.type = TOKEN_EOL; strcpy(tok.lexeme, "EOL"); break;
            case EOF: tok.type = TOKEN_EOF; strcpy(tok.lexeme, "EOF"); break;
            default: tok.type = TOKEN_UNKNOWN; strcpy(tok.lexeme, "?"); break;
        }
        read_char();
    }
    return tok;
}

