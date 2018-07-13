/**
 * lst_traverse() - iterates the list and applies the function to each link.
 * @node: The adress of a pointer to a linked list.
 * @func: The pointer to the function.
 *
 * The function lst_traverse() iterates the list(node) and applies
 * the given function(func) to each item of the list.
 *
 * The function pointed to by func will be used as follows:
 * (*func)(list_ptr->content);
 *
 * Return:
 * none
 */

#include "list.h"

void	lst_traverse(struct s_list **node, void (*func)(const void*))
{
	if (NULL == node || NULL == func)
		return ;

	struct s_list *tmp = *node;
	while (tmp)
	{
		func(tmp->content);
		tmp = tmp->next;
	}
}
