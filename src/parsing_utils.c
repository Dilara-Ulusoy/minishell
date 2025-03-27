/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:56:35 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/27 15:47:28 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(t_token_type type)
{
	if (type == TOKEN_AND || type == TOKEN_OR || type == TOKEN_PIPE
		|| type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE)
		return (1);
	return (0);
}

int	is_redir(t_token_type type)
{
	if (type == TOKEN_WORD || type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE)
		return (1);
	return (0);
}

/*
   get_precedence:
   - a simple numeric precedence system
	 PIPE (|) -> 10
   - higher => parse first
*/
int	get_precedence(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (10);
	else
		return (0);
}

/*
int get_precedence(t_token_type type)
{
    if (type == TOKEN_PIPE)  // Sadece | operatörü için öncelik döndürülür
        return (30);
    else
        return (0);  // Diğer operatörler için öncelik yoktur
}
*/

char	*resize_buffer(char *buffer, size_t *buffer_size)
{
	size_t	new_size;
	char	*new_buffer;

	new_size = *buffer_size * 2;
	new_buffer = ft_calloc(new_size, 1);
	if (!new_buffer)
	{
		free(buffer);
		return (NULL);
	}
	ft_memcpy(new_buffer, buffer, *buffer_size);
	free(buffer);
	*buffer_size = new_size;
	return (new_buffer);
}

/*
📌 append_to_buffer(buf, word_value)
Purpose: Appends a new word to a growing buffer,
resizing it dynamically if needed.

Steps:
1) Calculates the length of `word_value`.
2) Ensures the buffer has enough space:
   - If needed, calls `resize_buffer()` to increase `buf->size`.
   - If memory allocation fails, returns `0`.
3) Adds a space before appending `word_value`,
but only if it's **not the first word**.
4) Copies `word_value` into `buf->data`,
updates `buf->pos`, and null-terminates the string.
5) Returns `1` on success, `0` on failure.

Example:
- **Initial buffer:** `"echo"`
- `append_to_buffer(buf, "hello")` → `"echo hello"`
- `append_to_buffer(buf, "world")` → `"echo hello world"`

Effect:
- Ensures words are correctly spaced.
- Dynamically resizes the buffer when needed.
- Maintains a **null-terminated** string for safe usage.
*/
int	append_to_buffer(t_buffer *buf, const char *word_value)
{
	size_t	word_len;
	char	*tmp;

	if (!buf || !buf->data || !word_value)
		return (0);
	word_len = ft_strlen(word_value);
	while (buf->pos + word_len + 2 >= buf->size)
	{
		tmp = resize_buffer(buf->data, &(buf->size));
		if (!tmp)
			return (0);
		buf->data = tmp;
	}
	if (buf->pos > 0)
		buf->data[buf->pos++] = ' ';
	if (ft_memcpy(&buf->data[buf->pos], word_value, word_len) == NULL)
		return (0);
	buf->pos += word_len;
	buf->data[buf->pos] = '\0';
	return (1);
}
