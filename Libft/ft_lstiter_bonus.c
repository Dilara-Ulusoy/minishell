/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 14:01:08 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/29 14:01:12 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst ->content);
		lst = lst ->next;
	}
}
/*
This function iterates the linked list and applies
the function f to the content of each node.
First we check if the linked list and the function are valid.
Then we iterate the linked list.
Then we apply the function f to the content of the node.
Then we assign the next node to the current node
to reach the next node.
*/
