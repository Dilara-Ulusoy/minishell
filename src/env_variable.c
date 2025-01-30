#include "minishell.h"

// This function is used to calculate the length of an environment variable's value. Eg $PATH -> length of actual PATH string
int	get_env_var_length(const char *line)
{
	int			i;
	int			start;
	char		*var_name;
	const char	*env_value;

	i = 0;
	while (line[i] && line[i] != '$')
		i++;
	if (line[i] == '$')
		i++;
	start = i;
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	if (i == start)
		return (0);
	var_name =(char *)malloc(sizeof(char) * (i + 1));
	ft_memcpy(var_name, &line[start], i - start);
	var_name[i - start] = '\0';
	env_value = getenv(var_name);
	if (!env_value)
		return (0);
	return (ft_strlen(env_value));
}
// This function is used to extract the name of an environment variable from a string. Eg $HOME -> HOME
char	*extract_env_var_name(const char *line, int *index)
{
	int			i;
	int			start;
	char		*var_name;

	i = 0;
	while (line[i] && line[i] != '$')
		i++;
	if (line[i] == '$')
		i++;
	start = i;
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	if (i == start)
		return (0);
	var_name =(char *)malloc(sizeof(char) * (i + 1));
	ft_memcpy(var_name, &line[start], i - start);
	var_name[i - start] = '\0';
	*index = i;
	return (var_name);
}


