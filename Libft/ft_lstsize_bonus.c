/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 13:59:24 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/05/10 14:14:20 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	count;

	if (!lst)
		return (0);
	count = 0;
	while (lst)
	{
		lst = lst->next;
		count++;
	}
	return (count);
}
/*
This function counts the number of elements in a linked list.
We initialize the count variable to 0.
Then we iterate the linked list.
In each iteration, we increment the count variable by 1.
Finally, we return the count variable.
*/
/*
int main()
{
    // Create a linked list
    t_list *head = NULL;
    t_list *second = NULL;
    t_list *third = NULL;

    head = malloc(sizeof(t_list));
    second = malloc(sizeof(t_list));
    third = malloc(sizeof(t_list));

    head->data = 1;
    head->next = second;

    second->data = 2;
    second->next = third;

    third->data = 3;
    third->next = NULL;

    // Call the ft_lstsize function
    int size = ft_lstsize(head);

    // Print the size of the linked list
    printf("Size of the linked list: %d\n", size);

    return 0; 
*/
