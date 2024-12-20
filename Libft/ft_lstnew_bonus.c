/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:58:36 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/29 13:58:46 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node ->content = content;
	new_node ->next = NULL;
	return (new_node);
}
/*
This function creates a new node.
First we allocate memory for the new node.
Then we assign the content of the new node.
Then we assign the next node to NULL.
Then we return the new node.
*/
/*
int main()
{
    int data = 42;
    t_list *node = ft_lstnew(&data);
    printf("Content of the node: %d\n", *((int *)node->content));        
   	free(node);
    return (0);
}*/
