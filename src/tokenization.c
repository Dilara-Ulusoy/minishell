#include "minishell.h"


// Bu fonksiyon tokenları dolaşarak token tiplerini belirler
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
            if(contains_wildcard(current->value)) // Joker karakter kontrolü
                current->type = T_WORD_WILDCARD;
            else
            current->type = T_WORD;
        }
        current = current->next;
    }
}

// Stringi tokenlara ayırır ve token listesine ekler ve token tiplerini belirler
void tokenize(char *line, t_token **tokens)
{
    // split_tokens fonksiyonuyla tırnak içini koruyarak ayırıyoruz
    char **split = split_tokens(line);
    if (!split)
    {
        printf("Error: failed to split tokens\n");
        return;
    }
    // Her bir parçaya tipini belirleyip token listesine ekle
    int i = 0;
    while (split[i])
    {
        t_token_type type = get_token_type(split[i]);
        add_token(tokens, split[i], type); // Bu fonksiyon token type'i belirlendikten sonra tokeni listeye ekler
        i++;
    }
    // heap'te oluşturulmuş stringleri free et
    i = 0;
    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
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

// Handle quote toggling
static void handle_quote(char c, bool *in_single_quote, bool *in_double_quote)
{
    if (c == '\'' && !*in_double_quote)
    {
        *in_single_quote = !*in_single_quote;
    }
    else if (c == '\"' && !*in_single_quote)
    {
        *in_double_quote = !*in_double_quote;
    }
}

// Handle parenthesis tracking
static bool handle_parenthesis(char c, int *parenthesis_count, bool in_single_quote, bool in_double_quote)
{
    if (c == '(' && !in_single_quote && !in_double_quote)
    {
        (*parenthesis_count)++;
    }
    else if (c == ')' && !in_single_quote && !in_double_quote)
    {
        (*parenthesis_count)--;
        if (*parenthesis_count < 0) // Unmatched closing parenthesis
        {
            fprintf(stderr, "Syntax error: Unmatched closing parenthesis\n");
            return false;
            exit(128);
        }
    }
    return true;
}

char **split_tokens(const char *line)
{
    size_t max_tokens = strlen(line) + 1;
    char **tokens = (char **)malloc(sizeof(char *) * max_tokens);
    if (!tokens)
        return NULL;

    size_t token_count = 0;        // Number of tokens
    bool in_single_quote = false; // Are we inside single quotes?
    bool in_double_quote = false; // Are we inside double quotes?
    int parenthesis_count = 0;    // Track nested parentheses
    size_t i = 0;   // Index to traverse `line`
    size_t start = 0; // Start of the current token

    while (line[i] != '\0')
    {
        char c = line[i];

        // Handle quotes
        handle_quote(c, &in_single_quote, &in_double_quote);

        // Handle parentheses
        if (!handle_parenthesis(c, &parenthesis_count, in_single_quote, in_double_quote))
        {
            free(tokens);
            return NULL;
        }

        // Ignore special characters (backslash or semicolon) not required by the subject
        if ((c == '\\' || c == ';') && !in_single_quote && !in_double_quote)
        {
            i++; // Skip the character
            continue;
        }

        // Detect token boundaries on spaces or tabs
        if ((c == ' ' || c == '\t') && !in_single_quote && !in_double_quote && parenthesis_count == 0)
        {
            if (i > start) // If a token exists
            {
                tokens[token_count++] = strndup_custom(line + start, i - start);
            }
            start = i + 1; // Token starts after the space
        }

        i++;
    }

    // Check for unclosed quotes or parentheses
    if (in_single_quote || in_double_quote)
    {
        fprintf(stderr, "Syntax error: Unclosed quote detected\n");
        free(tokens);
        return NULL;
    }
    if (parenthesis_count > 0)
    {
        fprintf(stderr, "Syntax error: Unclosed parenthesis detected\n");
        free(tokens);
        return NULL;
    }

    // Add the last token if any
    if (i > start)
    {
        tokens[token_count++] = strndup_custom(line + start, i - start);
    }

    // Null-terminate the tokens array
    tokens[token_count] = NULL;

    return tokens;
}

