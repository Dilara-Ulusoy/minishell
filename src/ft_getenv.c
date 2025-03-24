#include "minishell.h"
#include "execution.h"

static char *copy_after_equal(char *envp)
{
	int i;
	int j;
	char *dest;

	i = 0;
	while (envp[i] != '=')
		i++;
	i++;
	j = 0;
	while (envp[i] != '\0')
	{
		i++;
		j++;
	}
	dest = malloc(sizeof(char) * (j + 1));
	i = 0;
	while (envp[i] != '=')
		i++;
	i++;
	j = 0;
	while (envp[i] != '\0')
	{
		dest[j] = envp[i];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

char *ft_getenv(char *var_name, char ***envp)
{
	int		i;
	char *name_w_equal;
	char *value;

	name_w_equal = ft_strjoin(var_name, "=");
	i = 0;
	while ((*envp)[i] != NULL && ft_strnstr((*envp)[i], name_w_equal, ft_strlen(name_w_equal)) == 0)
		i++;
	printf("Searched for: %s\n\n", name_w_equal);
	free(name_w_equal);
	if ((*envp)[i] == NULL)
	{
		printf("Var was not found in envp\n");
		return ("\0");
	}
	value = copy_after_equal((*envp)[i]);
	return (value); // Remember to free this later!
}