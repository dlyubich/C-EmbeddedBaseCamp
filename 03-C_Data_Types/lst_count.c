/**
 * lst_count() - returns the number of elements in a list,
 * which are equal to the given one.
 * @node: The adress of a pointer to a linked list.
 * @find: The pointer to a linked list.
 * @cmp: The pointer to a function.
 *
 * Takes as parametrs the adress of a pointer to a linked list, pointer to
 * a linked list and a function which will compare elements in the linked 
 * list(node) with given one element(find). If values are equal compare 
 * function returns 0.
 *
 * Return: 
 * integer number - OK
 * -1 - invalid data
 */

#include "list.h"

int	lst_count(struct s_list **node, struct s_list *find, int (*cmp)(const void*, const void*))
{
	if (NULL == node || NULL == find || NULL == cmp)
		return -1;

	struct s_list *tmp = *node;
	int counter = 0;
	
	while (tmp)
	{
		if (cmp(tmp->content, find->content) == 0)
			counter++;
		tmp = tmp->next;
	}
	return (counter);
}
