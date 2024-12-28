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
        free(minishell.line); // Bellek temizliği
    }
    return 0;
}
