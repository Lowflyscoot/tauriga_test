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
	char buf[20];

	sprintf(buf, "%d", list->data);
	return buf;
}
