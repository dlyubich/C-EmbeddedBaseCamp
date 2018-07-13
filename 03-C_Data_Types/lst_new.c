/**
 * lst_new() - allocates new linked list.
 * @content: The content to put in the new link.
 * @content_size: The size of the content of the new link.
 *
 * Allocates (with malloc(3)) and returns a “fresh” link. The
 * variables content(content) and content_size(content-size) of the
 * new link are initialized by copy of the parameters of the function.
 * If the parameter content is nul, the variable content is initialized to
 * NULL and the variable content_size is initialized to 0 even
 * if the parameter content_size isn’t. The variable next is
 * initialized to NULL. If the allocation fails, the function returns
 * NULL.
 *
 * Return:
 * the new link - OK
 * NULL - allocation fails
 */

#include "list.h"
#include <stdlib.h>

struct s_list	*lst_new(void const *content, size_t content_size)
{
	struct s_list *new;

	if (NULL == (new = malloc(sizeof(new))))
		return (NULL);
	if (NULL == content)
	{
		new->content = NULL;
		new->content_size = 0;
	}
	else
	{
		if (NULL == (new->content = malloc(sizeof(content) * content_size)))
			return (NULL);
		memcpy(new->content, content, content_size);
		new->content_size = content_size;
	}
	new->next = NULL;
	return (new);
}
