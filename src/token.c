#include "token.h"
#include <stdlib.h>

void token_free(Token *token) {
    if (token->value) {
        free(token->value);
        token->value = NULL;
    }
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_WORD: return "WORD";
        case TOKEN_PIPE: return "|";
        case TOKEN_AMPERDSAND: return "&";
        case TOKEN_SEMICOLON: return ";";
        case TOKEN_AND: return "&&";
        case TOKEN_OR: return "||";
        case TOKEN_LRAPEN: return "(";
        case TOKEN_RPAREN: return ")";
        case TOKEN_LESS: return "<";
        case TOKEN_GREATER: return ">";
        case TOKEN_DGREATER: return ">>";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}