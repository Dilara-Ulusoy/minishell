/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:02:28 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/05/07 14:52:04 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_calloc(size_t nitems, size_t size)
{
	size_t	i;
	size_t	totalsize;
	char	*outcome;

	totalsize = size * nitems;
	if (nitems != 0 && size > UINT_MAX / nitems)
	{
		return (NULL);
	}
	outcome = malloc(totalsize);
	if (!outcome)
		return (NULL);
	i = 0;
	while (i < totalsize)
	{
		outcome[i] = 0;
		i++;
	}
	return ((void *)outcome);
}
/*
This function allocates memory for an array of nitems elements based on 
the size of each element (size) and initializes all the memory to zero-byte.
If the multiplication of nitems and size is bigger than the 
maximum possible size for each item which memory can hold it (size_max / nitems),
then it means that memory required to fit all the items into memory is 
too big and the program runs of of memory.
*/
/*
 int main()
 {
    char first[] = "Hello";
    char *result;
 result = ft_calloc(sizeof(char),5);
 }
 */
