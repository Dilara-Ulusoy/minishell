#include "minishell.h"

static int	deal_evn(const char *line, int *index, int *result_index, char *result)
{
	char	*evn;
	int		env_len;
	int		result_size;

	result_size = ft_strlen(result);
	evn = handle_dollar_sign(line, (index), (*index));
	if (evn)
	{
		env_len = ft_strlen(evn);
		while((*result_index) + env_len >= result_size)
		{
			result_size *= 2;
			result = ft_realloc(result, result_size);
			if (!result)
			{
				free(evn);
				return (1);
			}
		}
		ft_strlcpy(result + (*result_index), evn, ft_strlen(evn) + 1); // Çözümlenmiş değişkeni kopyala
		(*result_index) += ft_strlen(evn); // result_index'i güncelle
		free(evn); // handle_dollar_sign'dan dönen bellek tahsisini temizle
	}
	return (0);
}

char *parse_quotes(const char *line, int *index)
{
	char quote;
	int result_index;
	char *result;

	quote = 0;
	result_index = 0;
	result = ft_calloc((ft_strlen(line) + 1), sizeof(char));
	if (!result)
		return (NULL);
	while (line[(*index)] && line[(*index)] != ' ')
	{
		if (line[(*index)] == '"' || line[(*index)] == '\'') // Quote karakteri mi?
		{
			if (quote == 0) // Henüz quote kaydedilmemişse, ilk quote'u al
			{
				quote = line[(*index)];
				(*index)++; // Açılış quote'unu atla
				continue;
			}
			else if (quote == line[(*index)]) // Kapatıcı quote bulundu
			{
				quote = 0; // Quote kapatıldı
				(*index)++; // Kapanış quote'unu atla
				continue;
			}
		}
		if (quote == '"' && line[(*index)] == '$')
		{
			if (deal_evn(line, index, &result_index, result) == 1)
				return (NULL);
			continue;
		}
		result[result_index++] = line[(*index)];
		(*index)++;
	}
	if (quote != 0) // Eğer açık quote kaldıysa
	{
		ft_putstr_fd("Unmatched quote\n", STDERR_FILENO);
		return (NULL);
	}
	result[result_index] = '\0'; // Sonuç stringini null-terminate et
	return (result);
}


/*
  char *parse_quotes(const char *line, int *index)
{
	int env_len = 0;
	int i = *index;
	int result_index = 0;
	char quote = 0;
	char *env;
	int result_size = ft_strlen(line) + 1;
	char *result = malloc(result_size);

	if (!result)
		return NULL;

	while (line[i] && !is_space(line[i]))
	{
		if (line[i] == '"' || line[i] == '\'') // Quote karakteri mi?
		{
			if (quote == 0) // Henüz quote kaydedilmemişse, ilk quote'u al
			{
				quote = line[i];
				i++; // Açılış quote'unu atla
				continue;
			}
			else if (quote == line[i]) // Kapatıcı quote bulundu
			{
				quote = 0; // Quote kapatıldı
				i++;	   // Kapanış quote'unu atla
				continue;
			}
		}
		if (quote == '"' && line[i] == '$')
		{
			env = handle_dollar_sign(line, &i, i); // Değişken çözümleme fonksiyonunu çağır
			if (env)
			{
				env_len = ft_strlen(env);
				while (result_index + env_len >= result_size)
				{
					result_size *= 2;
					result = ft_realloc(result, result_size);
					if (!result)
					{
						free(env);
						return NULL;
					}
				}
				ft_strlcpy(result + result_index, env, ft_strlen(env) + 1); // Çözümlenmiş değişkeni kopyala
				result_index += ft_strlen(env);								// result_index'i güncelle
				free(env);													// handle_dollar_sign'dan dönen bellek tahsisini temizle
			}
			continue;
		}
		result[result_index++] = line[i];
		i++;
	}
	if (quote != 0) // Eğer açık quote kaldıysa
	{
		ft_putstr_fd("Unmatched quote\n", STDERR_FILENO);
		return (NULL);
	}
	result[result_index] = '\0'; // Sonuç stringini null-terminate et
	*index = i;					 // Dış indexi güncelle
	return result;
} */

