/**
 * lst_extend() - add the content to existing list.
 * @node: The adress of a pointer to a linked list.
 * @extd: The pointer to a linked list.
 *
 * The function lst_extend() appends the contents of extd to node.
 * This function does not return any value but add the content
 * to existing list.
 *
 * Return: 
 * none
 */

#include "list.h"

void	lst_extend(struct s_list **node, struct s_list *extd)
{
	if (NULL == node || NULL == extd)
		return ;

	struct s_list *tmp = *node;

	if (tmp)
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = extd;
	}
	else
		*node = extd;
}
