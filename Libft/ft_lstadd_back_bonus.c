/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:37:53 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/05/10 13:44:04 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!new || !lst)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last -> next)
	{
		last = last -> next;
	}
	last -> next = new;
}
/*
This function adds the new node to the 
end of the linked list.
If the linked list is empty, then the 
new node is the first node of the linked list.
If the linked list is not empty,
then the new node is added to the end of the linked list.
*/
