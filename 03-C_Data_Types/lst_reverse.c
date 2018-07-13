/**
 * lst_reverse() - reverse the order of items in the list.
 * @node: The adress of a pointer to a linked list.
 *
 * The function lst_reverse() reverse the order of elements in the given
 * list(node).
 *
 * Return:
 * none
 */

#include "list.h"

void	lst_reverse(struct s_list **node)
{
	if (NULL == node)
		return ;
	
	struct s_list *tmp = *node;
	struct s_list *next, *prev = NULL;

	while (tmp)
	{
		next = tmp->next;
		tmp->next = prev;
		prev = tmp;
		tmp = next;
	}
}
