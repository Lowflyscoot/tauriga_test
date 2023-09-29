#include <stdio.h>

typedef struct list_s
{
        struct list_s *next; /* NULL for the last item in a list */
        int data;
}
list_t;

	/* Counts the number of items in a list.
	 */
int count_list_items(const list_t *head) {
    list_t *current_item = head;
    int num_of_elements = 1;
    while (current_item->next)
    {
        current_item = current_item->next;
        num_of_elements++;
    }
    return num_of_elements;
}

	/* Getting element inside the list by index.
	 */
int get_item_by_index(const list_t *head, const int index) {
    list_t *current_item = head;
    int number_of_element = 0;
    while (current_item)
    {
        if (number_of_element == index)
        {
            return current_item->data;
        }
        current_item = current_item->next;
        number_of_element++;
    }
    return 0;
}

    /* Inserts a new list item after the one specified as the argument.
	 */
void insert_next_to_list(list_t *item, int data) {
    // memory for next element of list
	// if (item->next)
	// {

	// }
    list_t *old_next_element = item->next;
    item->next = malloc(sizeof(list_t));
    item->next->next = old_next_element;
    item->next->data = data;    
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
	int num_els = count_list_items(list);
	char *buf = malloc(num_els);
	char buf2[20] = {};
	sprintf(buf, "%d", list->data);
	memcpy(buf2, buf, 20);
	return buf;
}



int main (void)
{
    list_t new_list;
    new_list.data = 10;
	new_list.next = NULL;
	insert_next_to_list(&new_list, 20);
	insert_next_to_list(&new_list, 11);
	insert_next_to_list(&new_list, 12);
	printf("%s", count_list_items(&new_list), *item_data(&new_list));
}