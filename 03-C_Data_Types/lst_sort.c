/**
 * lst_sort() - sorts the list using compare function.
 * @node: The adress of a pointer to a linked list.
 * @cmp: The pointer to a function.
 *
 * The function lst_sort() sorts the list given as a parameter(node), using the function 
 * pointer cmp(cmp) to select the order to apply.
 *
 * Functions passed as cmp have to return a value different from 
 * 0 if a and b are in the right order, 0 otherwise.
 *
 * Return:
 * none
 */

#include "list.h"

void	lst_sort(struct s_list **node, int (*cmp)(const void*, const void*))
{
	if (NULL == node || NULL == cmp)
		return ;

	struct s_list *tmp = *node;
	void *swap = NULL;

	while (tmp->next)
	{
		if (cmp(tmp->content, tmp->next->content) == 0)
		{
			swap = tmp->content;
			tmp->content = tmp->next->content;
			tmp->next->content = swap;
			tmp = *node;
		}
		else
			tmp = tmp->next;
	}
}


//Functions passed as cmp will always return a value different from 
//0 if a and b are in the right order, 0 otherwise.

