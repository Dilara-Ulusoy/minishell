#include "minishell.h"

/*****************************************************************************/
/*                      HELPER METHOD DECLARATIONS                            */
/*****************************************************************************/

/*
    Each helper method corresponds to a specific step in the tokenize process:
    1) parse_two_char_operator
    2) parse_single_char_operator
    3) parse_word
*/
/*
    Already-existing methods from the file:
    - is_two_char_operator
    - match_two_char_operator
    - create_new_token_range
    - append_token
    - skip_whitespace
    - is_space
    - match_single_char_operator
    - read_word_range
*/

/*****************************************************************************/
/*                   THE MAIN TOKENIZE FUNCTION (UPDATED)                    */
/*****************************************************************************/

/**
 * @brief tokenize
 * Splits the line string into a linked list of tokens.
 * Uses smaller helper methods for readability and memory correctness.
 *
 * @param line The user line (e.g. "echo hello && cat < file.txt").
 * @return Pointer to the first token in a linked list, or NULL if empty.
 */
t_token *tokenize(const char *line)
{
    t_token *head;
    int length;
    int i;

    if (!line || !*line)
        return(perror("Error: Empty line\n"), NULL);
    head = NULL;
    length = (int)ft_strlen(line);
    i = 0;
    while (i < length)
    {
        i = skip_whitespace(line, i);
        if (i >= length)
            break; // no more content left
        if (parse_two_char_operator(&head, line, &i) ||
            parse_single_char_operator(&head, line, &i) ||
            handle_newline(&head, line, &i))
            continue;
        if (!parse_word(&head, line, &i))
        {
            free_tokens(&head);
            return NULL; /* Return NULL to indicate failure */
        }
    }
    return head;
}

/*****************************************************************************/
/*                          PARSING FUNCTIONS                                */
/*****************************************************************************/

/**
 * @brief parse_two_char_operator
 * Checks if the current position in 'line' begins a two-char operator:
 *   "&&", "||", "<<" or ">>".
 * If so, it creates the corresponding token, appends it to '*head',
 * advances the parsing index, and returns 1.
 * If not, returns 0 and does not modify '*head' or '*pos' except for reading.
 */
int parse_two_char_operator(t_token **head, const char *line, int *pos)
{
    t_token_type doublechar;
    t_token *opToken;
    int length = (int)ft_strlen(line);
    int i = *pos;

    if (is_two_char_operator(line[i]) && (i + 1 < length))
    {
        doublechar = match_two_char_operator(line, i);
        if (doublechar != TOKEN_UNKNOWN)
        {
            /* e.g. "&&", "||", ">>", or "<<" */
            int operator_length = 3;
            /* Create the new token */
            opToken = create_new_token_range(doublechar, line, i, operator_length);
            append_token(head, opToken);
            /* Move pos forward by operator_length */
            *pos += operator_length;
            return 1; /* handled */
        }
    }
    return 0; /* not handled */
}

/**
 * @brief parse_single_char_operator
 * Checks if the current character in 'line' is a single-char operator:
 *   '<', '>', '|', '(', ')'
 * If so, creates a token, appends it to '*head', advances *pos, and returns 1.
 * Otherwise returns 0.
 */
int parse_single_char_operator(t_token **head, const char *line, int *pos)
{
    t_token_type single = match_single_char_operator(line[*pos]);
    int operator_length;
    if (single != TOKEN_UNKNOWN)
    {
        operator_length = 1;
        t_token *opToken = create_new_token_range(single, line, *pos, operator_length);
        append_token(head, opToken);

        *pos += operator_length; // skip this char
        return 1;                /* handled */
    }
    return 0; /* not handled */
}

/**
 * @brief parse_word
 * Reads a word (a sequence of non-operator, non-whitespace characters)
 * from the current position. If it successfully reads at least one character,
 * it creates a TOKEN_WORD, appends it to '*head', updates *pos, and returns 1.
 * If no characters are read (meaning we encountered an operator or whitespace
 * immediately), it returns 0.
 */
int parse_word(t_token **head, const char *line, int *pos)
{
    char *word = read_word_range(line, pos);
    if (!word)
        return 0; /* No word read */

    printf("Allocated Token: %p ------>%s\n", (void *)word, word);

    /* Joker karakter içeriyor mu? */
    if (ft_strchr(word, '*'))
    {
        if (!handle_wildcard_expansion(head, word))
        {
            free_tokens(head);
            return 0;
        }
        return 1; /* Successfully parsed */
    }
    /* Joker karakter yoksa, normal bir kelime olarak ekle */
    t_token *token = (t_token *)malloc(sizeof(t_token));
    if (!token)
    {
        free(word);
        free_tokens(head);
        return 0;
    }
    token->type = TOKEN_WORD;
    token->value = word;
    token->next = NULL;
    append_token(head, token);
    return 1; /* Successfully parsed */
}


/*****************************************************************************/
/*                              READ WORD RANGE                               */
/*****************************************************************************/

/**
 * @brief read_word_range
 *
 * Dynamically allocates memory for a "word" token by calculating the exact
 * word length first, then creating a buffer of the correct size. This
 * eliminates reliance on a fixed-size temporary array.
 *
 * How It Works (High-Level):
 *  1. We track the starting position (start = *index).
 *  2. We move through the line, stopping at:
 *     - Whitespace
 *     - Single-char operators ('(', ')', '|', '<', '>')
 *     - Characters that could start a two-char operator ('&', '|', '<', '>')
 *     - End of string
 *  3. Once we know where the word ends, we calculate wordLength = end - start.
 *  4. We dynamically allocate (wordLength + 1) bytes.
 *  5. We copy the substring from line[start...end - 1] into our newly allocated string.
 *  6. We return this string, and update *index to end.
 *
 * @param line The entire user line string (e.g., "echo hello && cat < file.txt").
 * @param index A pointer to an integer that holds the current scanning position.
 *              On exit, (*index) will be updated to the position just after
 *              the last character read for the word.
 *
 * @return A dynamically allocated string containing the characters of one "word",
 *         or NULL if no characters are read (meaning we stopped immediately
 *         because of an operator or whitespace).
 *
 * IMPORTANT:
 *  - You must free the returned string when you no longer need it, to avoid memory leaks.
 *  - This function does not create a token struct; it just returns the raw string.
 */
char *read_word_range(const char *line, int *index)
{
    int start; /* Remember where the word starts */
    int length;
    int wordLength;
    char c;
    char *processed;

    length = (int)ft_strlen(line);
    start = *index;
    while (*index < length)
    {
        c = line[*index];
        if (c == '"' || c == '\'')
        {
            processed = handle_quotes(line, index, c);
            if (!processed)
                return NULL; /* Syntax error: Unclosed quote */
            return processed; // İşlenmiş stringi geri döndür ve memory tahsisini yönet
        }
        if (is_space(c) || is_two_char_operator(c) || c == '(' || c == ')' || c == '\n')
            break;
        (*index)++;
    }
    wordLength = (*index) - start;
    if (wordLength == 0)
        return NULL;
    return allocate_word(line, start, wordLength);
}


int handle_newline(t_token **head, const char *line, int *pos)
{
    if (line[*pos] == '\n')
    {
        t_token *nl_token = create_new_token_range(TOKEN_NL, "\\n", 0, 2);
        append_token(head, nl_token);
        (*pos)++; // Yeni satırı geç
        return 1;
    }
    return 0;
}

/**
 * @brief handle_wildcard_expansion
 * Handles wildcard expansion for a given word by matching it against entries
 * in the current working directory and appending the results as tokens.
 *
 * @param head Pointer to the token list head.
 * @param word The word containing a wildcard.
 * @return 1 on success, 0 on failure.
 */
int handle_wildcard_expansion(t_token **head, char *word) {
    const char **matches = glob_pattern(get_cwd_entries(), word);
    if (matches) {
        while (*matches) {
            t_token *token = (t_token *)malloc(sizeof(t_token));
            if (!token) {
                free_tokens(head);
                return 0; /* Memory allocation failed */
            }
            token->type = TOKEN_WORD;
            token->value = ft_strdup(*matches); // Assign expanded value
            token->next = NULL;
            append_token(head, token); // Append token to the list
            matches++;
        }
    }
    free(word); // Free the original word
    return 1;
}