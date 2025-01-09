/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 10:40:08 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/05/14 14:19:13 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;
	char		*buffer;

	line = NULL;
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (free_stash_buffer_line(&stash, NULL, NULL));
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		free_stash_buffer_line(&stash, buffer, NULL);
		return (NULL);
	}
	stash = get_stash_from_buffer(fd, stash, buffer);
	if (!stash || *stash == 0)
		return (free_stash_buffer_line(&stash, NULL, NULL));
	line = extract_line(stash, line);
	if (!line)
		return (free_stash_buffer_line(&stash, NULL, NULL));
	stash = extract_remaining_stash(stash);
	if (!stash)
		return (free_stash_buffer_line(&stash, NULL, line));
	return (line);
}

char	*get_stash_from_buffer(int fd, char *stash, char *buffer)
{
	int		nbytes;
	char	*temp;

	nbytes = 1;
	if (!stash)
	{
		stash = ft_strdup("");
		if (!stash)
			return (free_stash_buffer_line(&stash, buffer, NULL));
	}
	while (nbytes > 0)
	{
		nbytes = read(fd, buffer, BUFFER_SIZE);
		if (nbytes == -1)
			return (free_stash_buffer_line(&stash, buffer, NULL));
		buffer[nbytes] = '\0';
		temp = ft_strjoin(stash, buffer);
		if (!temp)
			return (free_stash_buffer_line(&stash, buffer, NULL));
		stash = temp;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	free(buffer);
	return (stash);
}

char	*extract_line(char *stash, char *line)
{
	int	len;
	int	i;

	len = find_newline_index(stash);
	i = 0;
	if (stash == NULL)
		return (NULL);
	line = malloc((len + 1) * sizeof(char));
	if (!line)
		return (NULL);
	while (i < len)
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*extract_remaining_stash(char *stash)
{
	char	*remaining_stash;
	int		i;
	int		len;

	i = 0;
	if (stash == NULL)
		return (NULL);
	len = find_newline_index(stash);
	remaining_stash = (char *)malloc((ft_strlen(stash) - len + 1)
			* sizeof(char));
	if (!remaining_stash)
	{
		free(stash);
		return (stash = 0);
	}
	while (stash[len + i] != '\0')
	{
		remaining_stash[i] = stash[len + i];
		i++;
	}
	free(stash);
	remaining_stash[i] = 0;
	return (remaining_stash);
}

int	find_newline_index(char *stash)
{
	int	i;

	i = 0;
	while (stash[i] != '\n' && stash[i] != '\0')
		i++;
	if (stash[i] == '\n')
		i++;
	return (i);
}
