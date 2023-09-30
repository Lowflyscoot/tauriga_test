#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct list_s
{
        struct list_s *next; /* NULL for the last item in a list */
        int data;
}
list_t;

	/* Counts the number of items in a list.
	 */
int count_list_items(const list_t *head) {
    int num_of_elements = 1;
    list_t *next_element = head->next;
    while (next_element)
    {
        next_element = next_element->next;
        num_of_elements++;
    }
    return num_of_elements;
}

	/* Getting element inside the list by index.
	 */
int get_item_by_index(list_t *head, const int index) {
    int number_of_element = 0;
    list_t *next_element = head;
    while (next_element)
    {
        if (number_of_element == index)
        {
            return next_element->data;
        }
        next_element = next_element->next;
        number_of_element++;
    }
    return 0;
}

    /* Inserts a new list item after the one specified as the argument.
	 */
void insert_next_to_list(list_t *item, int data) {
    if (item->next)
	{
        list_t *old_next_element = item->next;
        item->next = malloc(sizeof(list_t));
        item->next->next = old_next_element;
        item->next->data = data;    
	}
    else
    {
        item->next = malloc(sizeof(list_t));
        item->next->data = data;    
        item->next->next = NULL;
    }
}

    /* Added element to end of list. Returns pointer to next element
	 */
list_t *add_to_list_end(list_t *item, int data) {
    list_t *current_element = item;
    while (current_element->next)
    {
        current_element = current_element->next;
    }
    current_element->next = malloc(sizeof(list_t));
    current_element->next->data = data;
    current_element->next->next = NULL;
    return current_element->next;
}

    /* Removes an item following the one specificed as the argument.
	 */
void remove_next_from_list(list_t *item) {
    if (item->next) {
        list_t* allocated_addr = item->next;
        item->next = item->next->next;
        free(allocated_addr);
    }
}

	/* Returns item data as text.
	 */
char *item_data(const list_t *list)
{
	char *buf = (char*)malloc(count_list_items(list));
	sprintf(buf, "%d", list->data);
	return buf;
}

int main (void)
{
    list_t new_list;
    new_list.data = 10;
	new_list.next = NULL;
    list_t *list_element = &new_list;
	list_element = add_to_list_end(list_element, 20);
	list_element = add_to_list_end(list_element, 11);
	list_element = add_to_list_end(list_element, 12);
	printf("%d", get_item_by_index(&new_list, 1));
}