/**
 * lst_headadd() - Adds the element at the beginning of the list.
 * @node: The address of a pointer to the first link of a list.
 * @new: The link to add at the beginning of the list.
 *
 * The function lst_headadd() adds a new elment(new) to head of 
 * the existing linked list(node).
 * This function only modifies the original list.
 * It doesn't return any value.
 *
 * Return:
 * none
 */

#include "list.h"

void	lst_headadd(struct s_list **node, struct s_list *new)
{
	if (NULL == node || NULL == new)
		return ;

	new->next = *node;
	*node = new;
}
