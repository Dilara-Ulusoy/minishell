/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 10:57:14 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/08/19 15:56:47 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <string.h>

char	*get_next_line(int fd);
char	*get_stash_from_buffer(int fd, char *stash, char *buffer);
char	*extract_line(char *stash, char *line);
char	*extract_remaining_stash(char *stash);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);
size_t	ft_strlen(const char *s);
int		find_newline_index(char *stash);
void	*free_stash_buffer_line(char **stash, char *buffer, char *line);

#endif
