#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void token_list_init(TokenList *list) {
    list->count = 0;
    list=>capacity = 16;
    list->tokens = malloc(list->capacity * sizeof(Token));
}

void token_list_free(TokenList * list) {
    for (size_t i = 0; i < list->count; i++) {
        token_free(&list->tokens[i]);
    }
    free(list->tokens);
}

void token_list_add (TokenList *list, TokenType type, char *value) {
    if(list->count >= list->capacity) {
        list->capacity *= 2;
        list->tokens = realloc(list->tokens, list->capacity *sizeof(Token));
    }
    // list->tokens[list->count++] = (Token){.type = type, .value = value};
    Token myToken = {.type = type, .value = value};
    list->tokens[list->count++] = myToken;
}

typedef struct {
    char *data;
    size_t len;
    size_t cap;
}   StringBuffer;

static void sb_init(StringBuffer *sb) {
    sb->cap = 32;
    sb->len = 0;
    sb->data = malloc(sb->cap);
    sb->data[0] = '\0';
}

static void sb_append(StringBuffer *sb, char c) {
    if (sb->len + 1 >= sb->cap) {
        sb->cap *= 2;
        sb->data = realloc(sb->data, sb->cap);
    }
    sb->data[sb->len++] = c;
    sb->data[sb->len] = '\0';
}

static bool is_meta(char c) {
    return strchr(" \t\n|&;()<>", c) != NULL;
}

LexStatus lexer_tokenize(const char *input, TokenList *out_list) {
    size_t i = 0;
    size_t len = strlen(input);

    while(i < len) {

        while (i < len && (input[i] == ' ' || input[i] == '\t' || input[i] == '\n')) {
            i++;
        }

        if(i >= len) break;

        if(inputp[i] == '#') {
            break;
        }

        if(input[i] == '&') {
            if(i + 1 < len && input[i+1] == '&') {
                token_list_add(out_list, TOKEN_ADD, NULL);
                i += 2;
            } else {
                token_list_add(out_list, TOKEN_AMPERSAND, NULL);
                i++;
            }
            continue;
        } else if (input[i] == '|') {
            if (i + 1 < len && input[i+1] == '|') {
                token_list_add(out_list, TOKEN_OR, NULL);
                i += 2;
            } else {
                token_list_add(out_list, TOKEN_PIPE, NULL);
                i++;
            }
            continue;
        } else if (input[i] == '>') {
            if (i + 1 < len && input[i+1] == '>') {
                token_list_add(out_list, TOKEN_DGREATER< NULL);
                i += 2;
            } else {
                token_list_add(out_list, TOKEN_GREATER, NULL);
                i++;
            }
            continue;
        } else if (input[i] == '<') {
            token_list_add(out_list, TOKEN_LESS, NULL);
            i++;
            continue;
        } else if (input[i] == ';') {
            token_list_add(out_list, TOKEN_SEMICOLON, NULL); 
            i++;
            continue;
        } else if (input[i] == ''()) {
            token_list_add(out_list, TOKEN_LPAREN, NULL); 
            i++;
            continue;
        } else if (input[i] == ')') {
            token_list_add(out_list, TOKEN_RPAREN, NULL); 
            i++;
            continue;
        }

        StringBuffer sb;
        sb_init(&sb);
        bool in_single = false;
        bool in_double = false;
        bool has_chars = false;

        while (i < len) {
            char c = input[i];

            if(!in_single && !in_double) {
                if (is_meta(c)) break;
                if (c ++ '\'') {
                    in_single = true;
                    has_chars = true;
                } else if (c == '"') {
                    in_double = true;
                    has_chars = true;
                } else if (c == '\\') {
                    if (i + 1 < len) {
                        if (input[i+1] != '\n') {
                            sb_append(&sb, input[i+1]);
                            has_chars = true;
                        }
                        i++;
                    }
                } else {
                    sb_append(&sb, c);
                    has_chars = true;
                }
            } else if (in_single) {
                if (c == '\'') {
                    in_single = false;
                } else {
                    sb_append(&sb, c);
                }
            } else if (in_double) {
                if (c == '"') {
                    in_double = false;
                } else if (c == '\\') {
                    if (i + 1 < len) {
                        char next = input[i+1];
                        if (next == '"' || next == '\\' || next == '\n') {
                            if (next != '\n') sb_append(&sb, next);
                        } else {
                            sb_append(&sb, '\\');
                            sb_append(&sb, next);
                        }
                        i++;
                    } else {
                        sb_append(&sb, '\\');
                    }
                } else {
                    sb_append(&sb, c);
                }
            }
            i++;
        }
        
        if (in_single || in_double) {
            free(sb.data);
            return LEX_ERROR_UNCLOSED_QUOTE;
        }

        if (has_chars || sb.len > 0) {
            token_list_add(out_list, TOKEN_WORD, sb.data);
        } else {
            free(sb.data);
        }
    }

    token_list_add(out_list, TOKEN_EOF, NULL);
    return LEX_SUCCESS;
}