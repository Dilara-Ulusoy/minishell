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
	dest[j] = '\0';
	return (dest);
}

char *ft_getenv(char *var_name, t_shell *shell)
{
	int		i;
	char *name_w_equal;
	char *value;

	// printf("var name: %s\n", var_name);
	// if (ft_strncmp(var_name,"?\0",2) == 0)
	// 	printf("%s\n", ft_itoa(shell->exit_code));
	name_w_equal = ft_strjoin(var_name, "=");
	i = 0;
	while ((*(shell->envp))[i] != NULL && ft_strnstr((*(shell->envp))[i], name_w_equal, ft_strlen(name_w_equal)) == 0)
		i++;
	//printf("Searched for: %s\n\n", name_w_equal);
	free(name_w_equal);
	if ((*(shell->envp))[i] == NULL)
	{
		//printf("Var was not found in envp\n");
		return ("\0");
	}
	value = copy_after_equal((*(shell->envp))[i]);
	return (value); // Remember to free this later!
}