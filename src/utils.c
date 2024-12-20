#include "minishell.h"

// Boşlukları temizleyen fonksiyon
char *trim_whitespace(char *str)
{
    char *start = str;
    char *end;

    // Baştaki boşlukları atla
    while (*start == ' ' || *start == '\t' || *start == '\n')
        start++;

    if (*start == '\0') // Eğer tümüyle boşluksa
        return ft_strdup(""); // Yeni boş bir string döndür

    // Sondaki boşlukları atla
    end = start + ft_strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t' || *end == '\n'))
        end--;

    // Tum bosluklardan arinmus yeni string oluştur.
    return (ft_substr(start, 0, end - start + 1));
}

// Kullanıcıdan girdi alan fonksiyon
char *get_input(void)
{
    char *line = readline("minishell> ");
    if (!line)
        return NULL;
    if (*line)
        add_history(line);
    char *trimmed_line = trim_whitespace(line);
    free(line);
    return trimmed_line; // Temizlenmiş string döndür
}