/**
 * lst_clear() - make the list empty.
 * @node: The adress of a pointer to a linked list.
 *
 * Takes as parametr the adress of a pointer to a linked list and frees
 * the memory of this linked list and every successors of that link using 
 * function free(3). Finally the pointer to the link that
 * was just freed set to NULL.
 * 
 * Return:
 * none
 */

#include "list.h"
#include <stdlib.h>

void	lst_clear(struct s_list **node)
{
	if (NULL == node)
		return ;
	
	struct s_list *tmp = *node;
	struct s_list *next;
	
	while (tmp != NULL)
	{
		next = tmp->next;
		if (tmp->content)
			free(tmp->content);
		free(tmp);
		tmp = next;
	}
	*node = NULL;
}
