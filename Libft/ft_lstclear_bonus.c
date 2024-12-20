/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:58:57 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/29 13:59:03 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		del((*lst)->content);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}
/*
This function deletes and frees the memory of the linked list.
First we assign the next pointer of the current node to the temporary node. 
We dont want to lose the next node otherwise we cannot reach the next node.
Then we delete the content of the current node.
Then we free the memory of the current node.
Then we assign the temporary node to the current node to reach the next node.
*/
