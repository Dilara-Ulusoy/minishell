#include "minishell.h"


static char	*handle_special_cases(const char *line, int *index)
{
	int	start;

	start = *index + 1;
	if (is_space(line[*index - 1]) && (line[*index + 1] == '\0' || is_space(line[*index + 1]))) // eg "echo $"
	{
		(*index)++;
		return (ft_strdup("$"));
	}
	if (is_space(line[*index - 1]) && !ft_isalpha(line[start]) && line[start] != '_') // eg "echo $123gegegeg"
	{
		(*index)++;
		while (!ft_isalpha(line[*index]) && line[*index] != '_' && line[*index] != '\0') // skip until the next word
			(*index)++;
		return ft_strdup("");
	}
	return NULL;
}

static char	*get_var_name(const char *line, int *index)
{
	int		start;
	int		len;

	start = *index;
	len = 0;
	while (ft_isalpha(line[start + len]) || line[start + len] == '_'
		|| ft_isdigit(line[start + len]))
		len++;
	if (len == 0)
		return (NULL);
	*index += len;
	return (ft_substr(line, start, len));
}

static void	append_env_value(char **result, char *var_name)
{
	char	*var_value;
	char	*temp;

	var_value = getenv(var_name);
	temp = *result;
	if (var_value != NULL)
		*result = ft_strjoin(*result, var_value);
	else
		*result = ft_strjoin(*result, "");
	free_this(temp, var_name, NULL, NULL);
}

static void	append_dollar_if_no_var(char **result)
{
	char	*temp;

	temp = *result;
	*result = ft_strjoin(*result, "$");
	if(!*result)
		*result = ft_strdup("");
	free(temp);
}

char	*get_env_var_value(const char *line, int *index)
{
	char	*result;
	char	*special;
	char	*var_name;

	special = handle_special_cases(line, index);
	if (special)
		return (special);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (line[*index] == '$')
	{
		(*index)++;
		var_name = get_var_name(line, index);
		if (!var_name)
			append_dollar_if_no_var(&result);
		else
			append_env_value(&result, var_name);
	}
	return (result);
}


