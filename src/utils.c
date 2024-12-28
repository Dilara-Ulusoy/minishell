#include "minishell.h"

// Kullanıcıdan girdi alan fonksiyon
char *get_input(void)
{
    char *line = readline("minishell$ ");
    if (!line)
        return NULL;
    if (*line)
        add_history(line);
    char *trimmed_line = ft_strtrim(line, " \t\n"); // Boşlukları temizle
    free(line);
    return trimmed_line; // Temizlenmiş string döndür
}