typedef struct list_s
{
        struct list_s *next; /* NULL for the last item in a list */
        int data;
}
list_t;

	/* Counts the number of items in a list.
	 */
int count_list_items(const list_t *head) {
	if (head->next) {
		return count_list_items(head->next) + 1;
	} else {
		return 1;
	}
}

    /* Inserts a new list item after the one specified as the argument.
	 */
void insert_next_to_list(list_t *item, int data) {
    // memory for next element of list
    item->next = malloc(sizeof(list_t));
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
	char buf[12];

    int num_of_elements = count_list_items(list);
    const list_t *next_list_item = list->next;
    buf[0] = (char)list->data;
    for (int i = 1; (i < num_of_elements && i < 12); i++)
    {
        buf[i] = next_list_item->data;
        if (!next_list_item->next) {
            break;
        }
        else {
            next_list_item = next_list_item->next;
        }
    }
	return buf;
}
