#include "minishell.h"


// Bu fonksiyon tokenları dolaşarak token tiplerini belirler
// Örneğin: ls | grep "hello" > output.txt
void determine_token_types(t_token *tokens)
{
    t_token *current = tokens;
    while (current)
    {
        if (current->is_operator)
        {
            current->type = get_token_type(current->value);
        }
        else if (is_group(current)) // t_group kontrolü
        {
            current->type = T_GROUP;
        }
        else
        {
            current->type = T_WORD;
        }
        current = current->next;
    }
}

// Stringi tokenlara ayırır ve token listesine ekler ve token tiplerini belirler
void tokenize(char *line, t_token **tokens)
{
    // split_tokens fonksiyonuyla tırnak içini koruyarak ayırıyoruz
    char **splitted = split_tokens(line);
    if (!splitted)
    {
        printf("Error: failed to split tokens\n");
        return;
    }
    // Her bir parçaya tipini belirleyip token listesine ekle
    int i = 0;
    while (splitted[i])
    {
        t_token_type type = get_token_type(splitted[i]);
        add_token(tokens, splitted[i], type);
        i++;
    }
    // heap'te oluşturulmuş stringleri free et
    i = 0;
    while (splitted[i])
    {
        free(splitted[i]);
        i++;
    }
    free(splitted);
    determine_token_types(*tokens);
}

// Token tipini belirleyen fonksiyon
t_token_type get_token_type(char *token)
{
    int len = ft_strlen(token);
    if (ft_strncmp(token, "|", 1) == 0 && len == 1)
        return T_PIPE;
    else if (ft_strncmp(token, ">", 1) == 0 && len == 1)
        return T_REDIRECT_OUT;
    else if (ft_strncmp(token, "<", 1) == 0 && len == 1)
        return T_REDIRECT_IN;
    else if (ft_strncmp(token, ">>", 2) == 0 && len == 2)
        return T_REDIRECT_APPEND;
    else if (ft_strncmp(token, "<<", 2) == 0 && len == 2)
        return T_REDIRECT_HEREDOC;
    else if (ft_strncmp(token, "&&", 2) == 0 && len == 2)
        return T_AND;
    else if (ft_strncmp(token, "||", 2) == 0 && len == 2)
        return T_OR;
    else if (ft_strncmp(token, "(", 1) == 0 && len == 1)
        return T_OPEN_P;
    else if (ft_strncmp(token, ")", 1) == 0 && len == 1)
        return T_CLOSE_P;
    else if (ft_strncmp(token, "\n", 1) == 0)
        return T_NL;
    else
        return T_WORD; // Varsayılan olarak argüman
}


/**
 * Basit bir strndup fonksiyonu (42 projesinde ft_strndup ya da benzeri bir fonksiyonunuz olabilir)
 */
static char *strndup_custom(const char *src, size_t n)
{
    char *dest = (char *)malloc(n + 1);
    if (!dest)
        return NULL;
    ft_memcpy(dest, src, n);
    dest[n] = '\0';
    return dest;
}

char **split_tokens(const char *line)
{
    size_t max_tokens = strlen(line) + 1;
    char **tokens = (char **)malloc(sizeof(char *) * max_tokens);
    if (!tokens)
        return NULL;

    size_t token_count = 0;        // Kaç adet token bulunduğunu takip eder
    bool in_single_quote = false; // Tek tırnak içinde miyiz?
    bool in_double_quote = false; // Çift tırnak içinde miyiz?
    bool in_parenthesis = false;  // Parantez içinde miyiz?
    size_t i = 0;   // line üzerinde gezer
    size_t start = 0; // token başlangıcı

    while (line[i] != '\0')
    {
        char c = line[i];

        // Eğer parantez açılıyorsa ve şu anda tırnak içinde değilsek
        if (c == '(' && !in_single_quote && !in_double_quote)
        {
            if (in_parenthesis)
            {
                fprintf(stderr, "Syntax error: Nested parentheses are not supported\n");
                free(tokens);
                return NULL;
            }

            in_parenthesis = true;
            start = i++; // Parantezin başladığı index
            continue;
        }

        // Parantez kapanıyorsa
        if (c == ')' && !in_single_quote && !in_double_quote)
        {
            if (!in_parenthesis)
            {
                fprintf(stderr, "Syntax error: Unmatched closing parenthesis\n");
                free(tokens);
                return NULL;
            }

            in_parenthesis = false;
            tokens[token_count++] = strndup_custom(line + start, i - start + 1);
            i++;
            start = i; // Parantezden sonraki pozisyondan devam et
            continue;
        }

        // Eğer tek tırnak açılmamışsa ve çift tırnak karakteri görürsek
        if (c == '\"' && !in_single_quote)
        {
            in_double_quote = !in_double_quote;
            i++;
            continue;
        }

        // Eğer çift tırnak açılmamışsa ve tek tırnak karakteri görürsek
        if (c == '\'' && !in_double_quote)
        {
            in_single_quote = !in_single_quote;
            i++;
            continue;
        }
        // Ignore special characters (backslash or semicolon) not required by the subject
        if ((c == '\\' || c == ';') && !in_single_quote && !in_double_quote)
        {
            i++; // Skip the character
            continue;
        }

        // Tırnak ve parantez dışında boşluk görürsek token sonunu işaretler
        if ((c == ' ' || c == '\t') && !in_single_quote && !in_double_quote && !in_parenthesis)
        {
            if (i > start) // Mevcut token var mı? (start < i ise var)
            {
                tokens[token_count++] = strndup_custom(line + start, i - start);
            }
            start = i + 1; // Boşluktan sonraki konumdan yeni token başlayacak
        }

        i++;
    }

    // Döngü sonunda hala tırnak içindeysek veya parantez kapanmadıysa syntax error döndür
    if (in_single_quote || in_double_quote)
    {
        fprintf(stderr, "Syntax error: Unclosed quote detected\n");
        free(tokens);
        return NULL;
    }
    if (in_parenthesis)
    {
        fprintf(stderr, "Syntax error: Unclosed parenthesis detected\n");
        free(tokens);
        return NULL;
    }

    // Döngü sonunda buffer'da kalan token'ı ekle
    if (i > start)
    {
        tokens[token_count++] = strndup_custom(line + start, i - start);
    }

    // Son eleman olarak NULL atayalım.
    tokens[token_count] = NULL;

    return tokens;
}

