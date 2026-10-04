#pragma once
#define TOKEN_H

typedef enum {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_AMPERSAND,
    TOKEN_SEMICOLON,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    LOKEN_LESS,
    TOKEN_GREATER,
    TOKEN_DGREATER,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokentType type;
    char *value;
} Token;

void token_free (Token *token);

const char* token_type_to_string(TokenType type);