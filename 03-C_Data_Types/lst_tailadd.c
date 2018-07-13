/**
 * lst_tailadd() - creates new element and appends it to the end of the list.
 * @node: The adress of a pointer to a linked list.
 * @content: The content to put in the new link.
 * @size: The size of the content of the new link.
 *
 * The function lst_tailadd() adds a new elment with given data(content)
 * and size(size) to the end of the existing linked list(node).
 * This function only modifies the original list.
 * It doesn't return any value.
 *
 * Return:
 * none
 */

#include "list.h"

void	lst_tailadd(struct s_list **node, void *content, size_t size)
{
	if (NULL == node || NULL == content)
		return ;
	
	struct s_list *listbegin = *node;
	
	if (listbegin)
	{
		while (listbegin->next)
			listbegin = listbegin->next;
		listbegin->next = lst_new(content, size);
	}
	else
		*node = lst_new(content, size);
}
