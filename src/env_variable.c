#include "minishell.h"

char *get_env_var_value(const char *line, int *index)
{
	int start;
	int len;
	char *var_name;
	char *var_value;

	start = *index + 1;
	len = 0;
	if (is_space(line[*index - 1]) && (line[*index + 1] == '\0' || is_space(line[*index + 1])))
	{
		(*index)++;
		return ft_strdup("$"); // Eğer `echo $` ise `$`'yi olduğu gibi koru
	}
	// Eğer önceki karakter boşluksa ve geçersiz karakter varsa, geçersiz karakterleri atla
	if (is_space(line[*index - 1]) && !ft_isalpha(line[start]) && line[start] != '_')
	{
		(*index)++; // `$` karakterini atla
		while (!ft_isalpha(line[*index]) && line[*index] != '_' && line[*index] != '\0')
			(*index)++; // Geçersiz karakterleri atla
		return ft_strdup("");
	}
	// Eğer geçersiz değişken başlıyorsa, sadece `$` döndür
	if (!ft_isalpha(line[start]) && line[start] != '_')
	{
		(*index)++;
		return ft_strdup("$");
	}
	while (ft_isalpha(line[start + len]) || line[start + len] == '_')
		len++;
	var_name = ft_substr(line, start, len);
	if (!var_name)
		return NULL;
	(*index) += len;
	var_value = getenv(var_name);
	free(var_name);
	(*index)++;
	if (!var_value)
		return ft_strdup("");
	return ft_strdup(var_value);
}
