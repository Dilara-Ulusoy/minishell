#include "minishell.h"

/*
 * Allocates a result for processing quoted strings.
 *
 * This function calculates the required result size for storing a quoted string,
 * taking into account any environment variable expansions. If environment variables
 * exist within the string, additional memory is allocated to accommodate their values.
 *
 * Parameters:
 * - `line` (const char *): The input string containing quoted content and potential environment variables.
 *
 * Returns:
 * - A pointer to the allocated result if successful.
 * - `NULL` if memory allocation fails, with an error message printed to STDERR.
 *
 * Memory Management:
 * - The caller is responsible for freeing the returned result after use.
 * - If environment variables are present, the result is allocated with extra space for expansion.
 */
static char *allocate_quote_result(const char *line)
{
	char *result;
	int len;
	int env_var_len;

	result = NULL;
	len = ft_strlen(line);
	env_var_len = get_env_var_length(line);
	if (env_var_len != 0)
	{
		result = (char *)malloc(len + env_var_len + 1);
		if (!result)
		{
			ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
			return (NULL);
		}
	}
	else
		result = (char *)malloc(len + 1);
	if (!result)
	{
		ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
		return (NULL);
	}
	return (result);
}

/*
 * Expands an environment variable within a quoted string.
 *
 * This function processes an environment variable found within a quoted string.
 * It extracts the variable name, retrieves its value using `getenv`, and replaces
 * the occurrence in the `result` result with the corresponding value.
 *
 * Parameters:
 * - `line` (const char *): The input string containing the environment variable.
 * - `i` (int *): Pointer to the current index in `line`, which is updated as the function processes the variable.
 * - `result` (char *): The output result where the expanded variable's value is copied.
 * - `res_index` (int *): Pointer to the current position in `result`, updated after copying the value.
 *
 * Returns:
 * - `1` if successful.
 * - `0` if memory allocation fails or if the variable name is invalid.
 *
 * Memory Management:
 * - The extracted variable name is dynamically allocated and freed within this function.
 * - The caller is responsible for ensuring `result` has enough space to store the expanded value.
 */
int expand_env_variable(const char *line, int *i, char *result, int *res_index)
{
	char *var_name;
	char *env_value;
	int len;

	var_name = extract_env_var_name(line, i);
	if (!var_name)
		return (0);
	env_value = getenv(var_name);
	free(var_name);
	if (!env_value)
		return (1);
	len = ft_strlen(env_value);
	ft_memcpy(&result[*res_index], env_value, len);
	*res_index += len;
	return (1);
}

/*
 * Processes the content within a quoted string.
 *
 * This function iterates through a quoted string, handling normal characters,
 * detecting the closing quote, and expanding environment variables if applicable.
 *
 * Parameters:
 * - `line` (const char *): The input string containing quoted content.
 * - `i` (int *): Pointer to the current index in `line`, updated as parsing progresses.
 * - `quote` (char): The type of quote being processed (single or double).
 * - `result` (char *): The output result to store the processed content.
 *
 * Returns:
 * - `1` if the quoted content is successfully processed and properly closed.
 * - `0` if an unclosed quote is encountered or if an error occurs during processing.
 *
 * Behavior:
 * - If a closing quote is found, it updates `result` and returns success.
 * - If inside double quotes and a `$` is encountered, it attempts to expand an environment variable.
 * - If the quote is never closed, it returns `0` to indicate an error.
 */
static int process_quoted_content(const char *line, int *i, char quote, char *result, int len)
{
	int res_index = 0;

	result[res_index++] = quote;		  // Açılış tırnağını ekle
	while (*i < len && line[*i] != quote) // Tırnak kapanana kadar devam et
	{
		if (quote == '"' && line[*i] == '$')
		{
			// Bir sonraki karakter geçerli bir değişken ismi başlatıyorsa env genişlet
			if (line[*i + 1] && (ft_isalpha(line[*i + 1]) || line[*i + 1] == '_'))
			{
				if (!expand_env_variable(line, i, result, &res_index))
					return 0;
			}
		}
		else
		{
			result[res_index++] = line[*i];
			(*i)++;
		}
	}
	if (*i >= len)
		return (0); // Tırnak kapanmadan döngü bitti
	result[res_index++] = quote; // Kapanış tırnağını ekle
	(*i)++;						 // Kapatıcı tırnağı atla
	// Eğer tırnak kapandıktan hemen sonra boşluk varsa burada bitir
	if (*i < len && line[*i] == ' ')
	{
		result[res_index] = '\0';
		return (1);
	}
	// Boşluk yoksa, kalan karakterleri kopyala (örn: `"ec""ho"`)
	while (*i < len && line[*i] != ' ')
	{
		result[res_index++] = line[*i];
		(*i)++;
	}
	result[res_index] = '\0'; // Null sonlandırıcı ekle
	return (1);
}

/*
 * Handles quoted content in a string.
 *
 * This function processes a quoted string by allocating the required result,
 * skipping the opening quote, and parsing the content inside. If the quote is not
 * properly closed, a syntax error is reported, and the program exits.
 *
 * Parameters:
 * - `line` (const char *): The input string containing quoted content.
 * - `i` (int *): Pointer to the current index in `line`, updated as the function progresses.
 * - `quote` (char): The type of quote being processed (single or double).
 *
 * Returns:
 * - A pointer to the allocated result containing the processed quoted content.
 * - `NULL` if memory allocation fails.
 *
 * Error Handling:
 * - If the quoted content is not properly closed, an error message is printed,
 *   the result is freed, and the program exits with status `1`.
 *
 * Memory Management:
 * - The caller is responsible for freeing the returned result after use.
 */
char *handle_quotes(const char *line, int *i, char quote)
{
	char *result;
	int len;

	len = ft_strlen(line);
	result = allocate_quote_result(line);
	if (!result)
	{
		ft_putstr_fd("Error; Memory allocation for quoted string failed\n", STDERR_FILENO);
		return (NULL);
	}
	(*i)++;
	if (!process_quoted_content(line, i, quote, result, len))
	{
		ft_putstr_fd("Error: Unclosed quote\n", STDERR_FILENO);
		free(result);
		return (NULL);
	}
	return (result);
}
#include <stdlib.h>
#include <stdio.h>

char *handle_env_var(const char *line, int *i)
{
    char *result;
    int env_var_len;
    int res_index = 0;

	result = NULL;
    env_var_len = get_env_var_length(line);
    result = (char *)malloc(env_var_len + 1);
    if (!result)
    {
        ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
        return NULL;
    }
    while (line[*i] && line[*i] != ' ')
    {
        if (line[*i] == '$')
        {
            if (line[*i + 1] && (ft_isalpha(line[*i + 1]) || line[*i + 1] == '_'))
            {
                if (!expand_env_variable(line, i, result, &res_index))
                {
                    free(result); // 🚨 Memory leak önleme
                    return ft_strdup("");
                }

        }
        result[res_index] = '\0';
    }
}
 return result;
}


