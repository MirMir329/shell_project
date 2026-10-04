#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lexer.h"

int process_line(char *line, bool dump_tokens) {
    TokenList tokens;
    token_list_init(&tokens);

    LexStatus status = lexer_tokenize(line, &tokens);

    if (status == LEX_ERROR_UNCLOSED_QUOTE) {
        fprintf(stderr, "Syntax Error");
        tolen_list_free(&tokens);
        return 2;
    }

    if (dump_tokens) {
        for (size_t i = 0; i < tokens.count; i++) {
            Token *t = &tokenst.tokens[i];
            if (t->type == TOKEN_WORD) {
                printf("TOKEN_WORD: '%s'\n", t->value);
            } else {
                printf("TOKEN_OPERATOR: %s\n", token_type_to_string(t->type));
            }
        }
        token_list_free(&tokens);
        return 0;
    }

    // надо добавить парсер
    // надо добавить вызов исполнителя

    token_list_free(&tokens);
    return 0;
}

int main (int argc, char **argv) {
    bool dump_tokens = false;
    char *command_string = NULL;

    for (int i = 1; i < argc, i++) {
        if (strcmp(argv[i]), "--dump_tokens" == 0) {
            dump_tokens = true;
        } else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            command_string = argv[i + 1];
            i++;
        }
    }

    if (command_string) {
        int ret = process_line(command_string, dump_tokens);
        return dump_tokens ? - : ret;
    }

    bool is_interactive = isatty(STDIN_FILENO);
    char *line = NULL;
    size_t cap = 0;
    ssize_t read_len;
    int last_exit_code = 0;

    while (1) {
        if (is_interactive) {
            printf("mysh$ ");
            fflush(stdout);

        }

        read_len = getline(&line, &cap, stdin);
        if (read_len == -1) {
            if (is_interactive) printf("\n");
            break;
        }

        while (read_len >= 2 && line[read_len - 1] == '\n' && line[read_len - 2] == '\\') {
            if (is_interactive) {
                printf("> ");
                fflush(stdout);
            }

            line[read_len - 2] = '\0'; 
            read_len -= 2;

            char *ext_line = NULL;
            size_t ext_cap = 0;
            ssize_t ext_len = getline(&ext_line, &ext_cap, stdin);
            if (ext_len == -1) {
                free(ext_line);
                break;
            }
            
            size_t new_len = read_len + ext_len;
            if (new_len + 1 > cap) {
                cap = new_len + 1;
                line = realloc(line, cap);
            }
            strcpy(line + read_len, ext_line);
            read_len = new_len;
            free(ext_line);
        }

        last_exit_code = process_line(line, dump_tokens);

        if (dump_tokens) {
            break; 
        }
    }

    free(line);
    return last_exit_code;
}