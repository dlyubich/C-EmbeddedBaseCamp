#include <string.h>

struct				s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
};

int					lst_count(struct s_list **node, struct s_list *find, int (*cmp)(const void*, const void*));
void				lst_clear(struct s_list **node);
void				lst_tailadd(struct s_list **node, void *content, size_t size);
void				lst_insert(struct s_list **node, struct s_list *new, size_t pos);
void				lst_remove(struct s_list **node, void *data, int (*cmp)(const void*, const void*));
void				lst_reverse(struct s_list **node);
void				lst_sort(struct s_list **node, int (*cmp)(const void*, const void*));
void				lst_traverse(struct s_list **node, void (*func)(const void*));
void				lst_headadd(struct s_list **node, struct s_list *new);
void				lst_extend(struct s_list **node, struct s_list *extd);
struct s_list		*lst_new(void const *content, size_t content_size);
