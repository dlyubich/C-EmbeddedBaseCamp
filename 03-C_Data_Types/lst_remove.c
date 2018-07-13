/**
 * lst_remove() - removes node from list.
 * @node: The adress of a pointer to a linked list.
 * @data: The pointer to a linked list.
 * @cmp: The pointer to a function.
 *
 * The function lst_remove() removes from linked list(node)
 * every node, which is equal to the given one(data).
 * Compare function(cmp) finds elements which are equal to the
 * needed value by comparing the current element of linked list
 * with given value(data). If values are equal it is expected
 * that function cmp returns 0.
 *
 * Return:
 * none
 */

#include "list.h"
#include <stdlib.h>

void	lst_remove(struct s_list **node, void *data, int (*cmp)(const void*, const void*))
{
	if (NULL == node || NULL == data || NULL == cmp)
		return ;

	struct s_list *tmp = *node;
	struct s_list *prev = NULL;
	
	while (tmp)
	{ 
		if (cmp(tmp->content, data) == 0)
		{
			if (prev)
			{
				prev->next = tmp->next;
				free(tmp->content);
				free(tmp);
				tmp = prev->next;
			}
			else
			{
				*node = tmp->next;
				free(tmp->content);
				free(tmp);
				tmp = *node;
			}
			continue;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}
