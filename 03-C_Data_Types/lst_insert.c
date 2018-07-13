/**
 * lst_insert() - inserts node into linked list at exact position.
 * @node: The adress of a pointer to a linked list.
 * @new: The pointer to a linked list.
 * @pos: The index of a position.
 *
 * The function lst_insert() puts node(new) in the linked list(node)
 * at the exact position(pos).
 * This method does not return any value but it inserts the given
 * element at the given index.
 *
 * Return:
 * none
 */

#include "list.h"

void	lst_insert(struct s_list **node, struct s_list *new, size_t pos)
{
	if (NULL == node || NULL == new)
		return ;
	if (pos == 0)
	{
		lst_headadd(node, new);
		return ;
	}
	
	struct s_list *tmp = *node;
	struct s_list *prev = NULL;
	size_t i = 0;

	while (tmp && i < pos)
	{
		prev = tmp;
		tmp = tmp->next;
		i++;
	}
	if (i == pos)
	{
		prev->next = new;
		new->next = tmp;
	}
}
