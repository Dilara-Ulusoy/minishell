#include "minishell.h"

/* A helper function to print the list of tokens */
void print_tokens(const t_token *head)
{
    const t_token *curr = head;
    while (curr)
    {
        printf("TOKEN: type=%d, value=\"%s\"\n", curr->type, curr->value);
        curr = curr->next;
    }
}

/* A helper function to free the entire token list */
void free_tokens(t_token **head)
{
    while (*head)
    {
        t_token *temp = (*head)->next;
        if ((*head)->value)
            free((*head)->value);
        free(*head);
        *head = temp;
    }
}

/* A helper function to test a single input line */
static void test_line(const char *line)
{
    printf("\n========================================================\n");
    printf("INPUT: \"%s\"\n", line);

    t_token *tokens = tokenize(line);
    if (!tokens)
    {
        printf("NO TOKENS PRODUCED (NULL)\n");
        return;
    }

    /* Print tokens */
    print_tokens(tokens);

    /* Free them */
    free_tokens(&tokens);
}

int main(void)
{
    /* 1. Empty or only whitespace */
    test_line("");
    test_line("    ");

    /* 2. Single word */
    test_line("ls");

    /* 3. Simple multiple words */
    test_line("ls -l");

    /* 4. Mixed operators (two-char and single-char) */
    test_line("echo hello && cat < file.txt");
    test_line("true||false");
    test_line("cmd << INFILE >> OUTFILE");

    /* 5. Single-char operator tests */
    test_line("(echo)");
    test_line("ls > out.txt");
    test_line("ls|grep txt");

    /* 6. Edge case: consecutive operators with no space */
    test_line("&& echo next");
    test_line("||echoNoSpace");
    test_line("<<<some");
    test_line(">>>other");

    /* 7. Words with unusual characters */
    test_line("command???args");
    test_line("command@@@###");
    test_line("cmd123");

    /* 8. Operator at start or end */
    test_line("&&echo start");
    test_line("echo end||");

    /* 9. Just operators */
    test_line(">>");
    test_line("&&");

    /* 10. Complex combination */
    test_line("cat <<EOF && echo done (test) || ls -l");

    printf("\nAll tests completed.\n");
    return 0;
}