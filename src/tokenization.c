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
    if (!line || !*line)
        return NULL;

    t_token *head = NULL;   /* head of token list */
    int length = (int)ft_strlen(line);
    int i = 0;

    while (i < length)
    {

        i = skip_whitespace(line, i);
        if (i >= length)
            break; // no more content left

        /*
            2. Check for two-char operators if possible.
               parse_two_char_operator will return 1 if it successfully parsed
               a two-char operator token and appended it to 'head', otherwise 0.
        */
        if (parse_two_char_operator(&head, line, &i))
            continue;

        /*
            3. Single-char operator?
               parse_single_char_operator will return 1 if it parsed a single-char
               operator, otherwise 0.
        */
        if (parse_single_char_operator(&head, line, &i))
            continue;

        /*
            4. Otherwise, read a "word" until we reach operator/whitespace.
               parse_word will return 1 if a word was parsed, 0 if not.
               If 0 is returned, it means we found no characters for a word
               (perhaps an immediate operator?), so we just continue.
        */
        if (!parse_word(&head, line, &i))
        {
            /* If parse_word fails (e.g., due to a syntax error), clean up tokens */
            free_tokens(&head);
            return NULL; /* Return NULL to indicate failure */
        }
    }
    return head;
}

/*****************************************************************************/
/*                      STEP 2: PARSE TWO-CHAR OPERATOR                       */
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
    int length = (int)ft_strlen(line);
    int i = *pos;

    if (is_two_char_operator(line[i]) && (i + 1 < length))
    {
        t_token_type twoCharType = match_two_char_operator(line, i);
        if (twoCharType != TOKEN_UNKNOWN)
        {
            /* e.g. "&&", "||", ">>", or "<<" */
            int operatorLength = 2;

            /* Create the new token */
            t_token *opToken = create_new_token_range(twoCharType, line, i, operatorLength);
            append_token(head, opToken);

            /* Move pos forward by operatorLength */
            *pos += operatorLength;
            return 1; /* handled */
        }
    }
    return 0; /* not handled */
}

/*****************************************************************************/
/*                     STEP 3: PARSE SINGLE-CHAR OPERATOR                     */
/*****************************************************************************/

/**
 * @brief parse_single_char_operator
 * Checks if the current character in 'line' is a single-char operator:
 *   '<', '>', '|', '(', ')'
 * If so, creates a token, appends it to '*head', advances *pos, and returns 1.
 * Otherwise returns 0.
 */
int parse_single_char_operator(t_token **head, const char *line, int *pos)
{
    t_token_type singleType = match_single_char_operator(line[*pos]);
    if (singleType != TOKEN_UNKNOWN)
    {
        int operatorLength = 1;
        t_token *opToken = create_new_token_range(singleType, line, *pos, operatorLength);
        append_token(head, opToken);

        *pos += operatorLength; // skip this char
        return 1; /* handled */
    }
    return 0; /* not handled */
}

/*****************************************************************************/
/*                          STEP 4: PARSE A WORD                              */
/*****************************************************************************/

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
        return 0; /* no word read */

    /* Build the token from the newly allocated string 'word'. */
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
    return 1; /* handled */
}


/*
   is_two_char_operator:
   Returns non-zero (true) if 'c' can start a two-char operator like:
    - '&' for "&&"
    - '|' for "||"
    - '<' for "<<"
    - '>' for ">>"
*/
int is_two_char_operator(char c)
{
    return (c == '&' || c == '|' || c == '<' || c == '>');
}

/*
   match_two_char_operator:
   - Looks at line[index] and line[index+1].
   - Returns a token_type if it matches "&&", "||", "<<" or ">>".
   - Otherwise returns TOKEN_UNKNOWN.
*/
t_token_type match_two_char_operator(const char *line, int index)
{
    if (line[index] == '&' && line[index + 1] == '&')
        return TOKEN_AND;
    if (line[index] == '|' && line[index + 1] == '|')
        return TOKEN_OR;
    if (line[index] == '<' && line[index + 1] == '<')
        return TOKEN_REDIR_HERE;
    if (line[index] == '>' && line[index + 1] == '>')
        return TOKEN_REDIR_APPEND;
    return TOKEN_UNKNOWN;
}

/*
   match_single_char_operator:
   - Checks if 'c' is one of the known single-character operators.
   - If it is, return the corresponding token_type.
   - If not, return TOKEN_UNKNOWN.
*/
t_token_type match_single_char_operator(char c)
{
    if (c == '<') return TOKEN_REDIR_IN;
    if (c == '>') return TOKEN_REDIR_OUT;
    if (c == '|') return TOKEN_PIPE;
    if (c == '(') return TOKEN_PAREN_OPEN;
    if (c == ')') return TOKEN_PAREN_CLOSE;
    return TOKEN_UNKNOWN;
}

/*****************************************************************************/
/*                          CREATE / APPEND TOKENS                            */
/*****************************************************************************/

/**
* @brief create_new_token_range
* Allocates a new token, copies the substring of 'line' from startIndex
* (inclusive) to (startIndex + length) (exclusive) into newTok->value,
* sets newTok->type, and ensures newTok->next is NULL.
*
* @param type       The token type to assign (e.g., TOKEN_AND, TOKEN_WORD, etc.)
* @param line      The entire line string
* @param startIndex The position where the substring begins
* @param length     The length of the substring to copy
* @return A pointer to the newly allocated t_token, or NULL if memory fails.
*/
t_token *create_new_token_range(t_token_type type,
                                const char *line,
                                int startIndex,
                                int length)
{
    // 1. Allocate memory for the token structure
    t_token *newTok = (t_token *)malloc(sizeof(t_token));
    if (!newTok)
        return NULL; // memory error

    newTok->type = type;

    // 2. Allocate memory for the token's value string
    newTok->value = (char *)malloc(length + 1); // +1 for '\0'
    if (!newTok->value)
    {
        free(newTok);
        return NULL;
    }

    // 3. Copy the substring from line into newTok->value
    //    We'll use strncpy here for demonstration, but be mindful of bounds in real code.
    strncpy(newTok->value, &line[startIndex], length);
    newTok->value[length] = '\0'; // null terminate

    // 4. Initialize next pointer
    newTok->next = NULL;

    return newTok;
}

/*
   append_token:
   - Adds 'new_token' to the end of the linked list whose head is '*head'.
   - If *head is NULL, then new_token becomes the first and only element.
*/
void append_token(t_token **head, t_token *new_token)
{
    if (!new_token)
        return;

    if (!*head)
    {
        /* If the list is empty, new_token is the new head. */
        *head = new_token;
        return;
    }
    t_token *cursor = *head;
    while (cursor->next)
        cursor = cursor->next; /* move to the end of the list */

    cursor->next = new_token; /* append at the end */
}

int skip_whitespace(const char *line, int i)
{
    while (line[i] && is_space(line[i]))
        i++;
    return i;
}

int is_space(char c)
{
    if(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f')
        return 1;
    return 0;
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
    int start = *index; /* Remember where the word starts */
    int length = (int)ft_strlen(line);
    int end;
    int wordLength;

    while (*index < length)
    {
        char c = line[*index];
        if (c == '"' || c == '\'')
        {
            if (!handle_quotes(line, index, c))
                return NULL; /* Syntax error: Unclosed quote */
            continue;
        }
        if (is_space(c) || is_two_char_operator(c) || c == '(' || c == ')')
            break;
        (*index)++;
    }
    end = *index; /* Where the word ended */
    wordLength = end - start;
    if (wordLength == 0)
        return NULL;
    return allocate_word(line, start, wordLength);
}


int handle_quotes(const char *line, int *index, char quote)
{
    int length = (int)ft_strlen(line);

    /* Move past the opening quote. */
    (*index)++;

    /* Traverse until a matching closing quote is found. */
    while (*index < length)
    {
        if (line[*index] == quote) /* Matching closing quote found */
        {
            (*index)++;
            return 1; /* Successfully handled the quote */
        }
        (*index)++;
    }
    /* No matching closing quote found */
    printf("Syntax error: Unclosed quote\n");
    return 0;
}
char *allocate_word(const char *line, int start, int length)
{
    char *word;

    word = (char *)malloc(length + 1);
    if (!word)
        return NULL;
    ft_memcpy(word, &line[start], length);
    word[length] = '\0';
    return word;
}