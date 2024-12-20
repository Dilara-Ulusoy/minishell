#include "minishell.h"


int main(int argc, char **argv, char **envp)
{
    t_shell minishell;
    t_token *tokens = NULL;

    (void)argv;
    (void)envp;

    if(argc != 1)
    {
        ft_putendl_fd("Too many args, Usage: ./minishell", 2);
        exit(1);
    }
    while (1)
    {
        minishell.line = get_input();
        if (!minishell.line) // Kullanıcı CTRL+D ile çıkarsa.
        {
            printf("Exit\n");
            break;
        }
        if (*(minishell.line) == '\0') { // Boş girdi durumunda devam et
            free(minishell.line); // Önceki belleği temizle
            continue;
        }
        tokenize(minishell.line, &tokens);
        print_tokens(tokens);
        free_tokens(tokens);
        tokens = NULL;
        printf("You entered: %s\n", minishell.line);
        free(minishell.line); // Bellek temizliği
    }
    return 0;
}
/*
void tokenize(char *line, t_token **tokens, t_parser_state *parser_state) {
    int i = 0;

    while (line[i]) {
        // Boşlukları atla
        while (line[i] && isspace(line[i]))
            i++;

        if (line[i] == '\0')
            break;

        // Tırnaklı ifadeleri işleme
        if (line[i] == '"' || line[i] == '\'') {
            char quote = line[i++];
            int start = i;
            while (line[i] && line[i] != quote)
                i++;
            if (line[i] == quote) {
                char *quoted = ft_substr(line, start, i - start);
                if (parser_state->expect_command) {
                    add_token(tokens, quoted, T_COMMAND);
                    parser_state->expect_command = false;
                } else {
                    add_token(tokens, quoted, T_ARG);
                }
                free(quoted);
                i++;
            } else {
                printf("Syntax error: unclosed quote\n");
                return;
            }
        }
        // Parantezleri işleme
        else if (line[i] == '(') {
            add_token(tokens, "(", T_OPEN_P); // Açma parantez
            parser_state->expect_command = true; // Parantez sonrası komut beklenir
            i++;
        } else if (line[i] == ')') {
            add_token(tokens, ")", T_CLOSE_P); // Kapatma parantez
            parser_state->expect_command = false; // Parantez kapandıktan sonra argüman olabilir
            i++;
        }
        // Operatörleri işleme
        else if (is_operator_char(line[i])) {
            int start = i;
            while (line[i] && is_operator_char(line[i]))
                i++;
            char *operator = ft_substr(line, start, i - start);
            add_token(tokens, operator, get_operator_type(operator));
            parser_state->expect_command = true; // Operatör sonrası yeni komut bekleniyor
            free(operator);
        }
        // Normal kelime
        else {
            int start = i;
            while (line[i] && !isspace(line[i]) && line[i] != '(' && line[i] != ')' && !is_operator_char(line[i]))
                i++;
            char *word = ft_substr(line, start, i - start);
            if (parser_state->expect_command) {
                add_token(tokens, word, T_COMMAND); // Komut olarak işlenir
                parser_state->expect_command = false; // Artık argüman bekleniyor
            } else {
                add_token(tokens, word, T_ARG); // Argüman
            }
            free(word);
        }
    }

    // Token tiplerini düzenle
    determine_token_types(*tokens, parser_state);
}

*/