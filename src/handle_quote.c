#include "minishell.h"

 char *parse_quotes(const char *line, int *index)
{
	int env_len = 0;
	int i = *index;
	int result_index = 0;
	char quote = 0;
	char *evn;
	int result_size = ft_strlen(line) + 1;
	char *result = malloc(result_size);

	if (!result)
		return NULL;

	while (line[i] && line[i] != ' ')
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
				i++; // Kapanış quote'unu atla
				continue;
			}
		}
		if (quote == '"' && line[i] == '$')
		{
			evn = handle_dollar_sign(line, &i, i); // Değişken çözümleme fonksiyonunu çağır
			if (evn)
			{
				env_len = ft_strlen(evn);
				while(result_index + env_len >= result_size)
				{
					result_size *= 2;
					result = ft_realloc(result, result_size);
					if (!result)
					{
						free(evn);
						return NULL;
					}
				}
				ft_strlcpy(result + result_index, evn, ft_strlen(evn) + 1); // Çözümlenmiş değişkeni kopyala
				result_index += ft_strlen(evn); // result_index'i güncelle
				free(evn); // handle_dollar_sign'dan dönen bellek tahsisini temizle
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
	*index = i; // Dış indexi güncelle
	return result;
}
