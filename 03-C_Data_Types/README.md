# LIBLST

**Linked list library**

Simple library to work with linked lists.

Linked list element is defined as following struct:

`struct				s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
};`

**Supports next operations:**

• lst_traverse – iterate through list applying function to its nodes;
• lst_clear – make the list empty;
• lst_tailadd – appends one element to the end of the list;
• lst_insert – put node at exact position in the list;
• lst_remove – removes node from list;
• lst_count – returns the number of elements in a list, which are equal to the given one;
• lst_reverse – reverse the order of items in the list;
• lst_sort – given the compare function sorts list items (ascending or descending);
• lst_extend - add the content to existing list;
• lst_headadd - add the element at the beginning of the list;
• lst_new - allocates new linked list. 