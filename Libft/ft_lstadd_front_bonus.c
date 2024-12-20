/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:42:03 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/29 13:40:24 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!new || !lst)
		return ;
	new->next = *lst;
	*lst = new;
}
/*
This function adds the new node to the 
beginning of the linked list. We assign the next 
pointer of the new node to the current node of the linked list.
Then we assign the first node of the linked list to the new node.
Then the new node becomes the first node of the linked list.
*/
