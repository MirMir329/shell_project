#pragma once
#define LEXER_H

#include "token.h"
#include <stddef.h>

typedef struct {
    Token *tokens;
    size_t count;
    size_t capacity;
} TokenList;

typedef enum {
    LEX_SUCCESS,
    LEX_ERROR_UNCLOSED_QUOTE,
    LEX_ERROR_MEMORY
} LexStatus;

void token_list_init(TokenList *list);
void token_list_free(TokenList *list);
void token_list_add(TokenList *list, TokenType type, char *value);

LexStatus lexer_tokenize(const char *input, TokenList *out_list);
