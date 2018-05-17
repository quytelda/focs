/* double_list.c - Doubly Linked List Implementation
 * Copyright (C) 2018 Quytelda Kahja
 *
 * This file is part of focs.
 *
 * focs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * focs is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with focs.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "list/single_list.h"

static struct sl_element * __create_element(const void * data,
	                                    const size_t data_size)
{
	struct sl_element * elem;

	elem = malloc(sizeof(*elem));
	if(!elem)
		return_with_errno(ENOMEM, NULL);

	elem->data = malloc(data_size);
	if(!elem->data)
		goto_with_errno(ENOMEM, exit);

	memcpy(elem->data, data, data_size);
	return elem;

exit:
	free(elem);
	return NULL;
}

static struct sl_element * __lookup(const single_list list, const size_t pos)
{
	struct sl_element * current;

	if(pos >= DS_PRIV(list)->length)
		return NULL;

	current = DS_PRIV(list)->head;
	for(size_t i = 0; i < pos; i++)
		current = current->next;
	return current;
}

static void __push_head(single_list list, struct sl_element * current)
{
	current->next = DS_PRIV(list)->head;
	DS_PRIV(list)->head = current;

	if(!DS_PRIV(list)->tail)
		DS_PRIV(list)->tail = current;

	(DS_PRIV(list)->length)++;
}

static void __push_tail(single_list list, struct sl_element * current)
{
	if(DS_PRIV(list)->tail)
		DS_PRIV(list)->tail->next = current;

	current->next = NULL;
	DS_PRIV(list)->tail = current;

	if(!DS_PRIV(list)->head)
		DS_PRIV(list)->head = current;

	(DS_PRIV(list)->length)++;
}

static struct sl_element * __pop_head(single_list list)
{
	struct sl_element * current;

	if(DS_PRIV(list)->length == 0)
		return NULL;

	current = DS_PRIV(list)->head;
	DS_PRIV(list)->head = current->next;

	if(!DS_PRIV(list)->head)
		DS_PRIV(list)->tail = NULL;

	(DS_PRIV(list)->length)--;

	return current;
}

static struct sl_element * __pop_tail(single_list list)
{
	struct sl_element * current;
	struct sl_element * prev;

	if(DS_PRIV(list)->length == 0)
		return NULL;

	/* Find the previous element. */
	linked_list_while(list, prev, prev->next != DS_PRIV(list)->tail) {}

	current = DS_PRIV(list)->tail;
	DS_PRIV(list)->tail = prev;

	if(DS_PRIV(list)->tail)
		DS_PRIV(list)->tail->next = NULL;
	else
		DS_PRIV(list)->head = NULL;

	(DS_PRIV(list)->length)--;

	return current;
}

static bool __insert(single_list list,
	             struct sl_element * current,
	             const size_t pos)
{
	struct sl_element * prev;

	if(pos > DS_PRIV(list)->length)
		return false;

	if(pos == 0) {
		__push_head(list, current);
	} else if(pos == DS_PRIV(list)->length) {
		__push_tail(list, current);
	} else {
		prev = __lookup(list, pos - 1);
		current->next = prev->next;
		prev->next = current;

		(DS_PRIV(list)->length)++;
	}

	return true;
}

static struct sl_element * __remove_element(single_list list, size_t pos)
{
	struct sl_element * current;
	struct sl_element * prev;

	if(pos >= DS_PRIV(list)->length)
		return NULL;

	if(pos == 0) {
		current = __pop_head(list);
	} else if(pos == DS_PRIV(list)->length - 1) {
		current = __pop_tail(list);
	} else {
		prev = __lookup(list, pos - 1);
		current = prev->next;
		prev->next = current->next;

		(DS_PRIV(list)->length)--;
	}

	return current;
}

static void __delete_element(single_list list, struct sl_element * elem)
{
	struct sl_element * prev;

	linked_list_while(list, prev, prev->next != elem) {}

	/* Fix head and tail. */
	if(DS_PRIV(list)->head == elem)
		DS_PRIV(list)->head = elem->next;
	if(DS_PRIV(list)->tail == elem)
		DS_PRIV(list)->tail = prev;

	if(prev)
		prev->next = elem->next;

	(DS_PRIV(list)->length)--;
}

static void __delete_before(single_list list, struct sl_element * mark)
{
	struct sl_element * current;

	linked_list_while_safe(list, current, current != mark) {
		free(current->data);
		free(current);

		(DS_PRIV(list)->length)--;
	}

	DS_PRIV(list)->head = mark;
	if(!mark)
		DS_PRIV(list)->tail = NULL;
}

static void __delete_after(single_list list, struct sl_element * mark)
{
	bool passover = true;
	struct sl_element * current;

	linked_list_foreach_safe(list, current) {
		if(!passover || !mark) {
			free(current->data);
			free(current);

			(DS_PRIV(list)->length)--;
		}

		if(current == mark)
			passover = false;
	}

	DS_PRIV(list)->tail = mark;
	if(mark)
		mark->next = NULL;
	else
		DS_PRIV(list)->head = NULL;
}

void __reverse(single_list list)
{
	struct sl_element * current;
	struct sl_element * tmp = NULL;

	linked_list_foreach_safe(list, current) {
		current->next = tmp;
		tmp = current;
	}

	/* Swap the list head and tail */
	tmp = DS_PRIV(list)->head;
	DS_PRIV(list)->head = DS_PRIV(list)->tail;
	DS_PRIV(list)->tail = tmp;
}

single_list sl_create(const struct ds_properties * props)
{
	single_list list;
	struct single_list_priv * priv;

	list = malloc(sizeof(*list));
	if(!list)
		return_with_errno(ENOMEM, NULL);

	DS_INIT(list, props, &mgmt_ops, &hof_ops);

	priv = DS_PRIV(list);
	priv->head = NULL;
	priv->tail = NULL;
	priv->length = 0;

	if(rwlock_alloc(&priv->rwlock) < 0)
		goto exit;

	return list;

exit:
	sl_destroy(&list);

	return NULL;
}

void sl_destroy(single_list * list)
{
	struct sl_element * current;

	rwlock_writer_entry(DS_PRIV(*list)->rwlock);

	linked_list_foreach_safe(*list, current) {
		free(current->data);
		free(current);
	}

	rwlock_writer_exit(DS_PRIV(*list)->rwlock);
	rwlock_free(&DS_PRIV(*list)->rwlock);

	DS_FREE(list);
}

bool sl_empty(single_list list)
{
	bool null;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);
	null = (DS_PRIV(list)->length == 0);
	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return null;
}

bool sl_elem(single_list list, const void * data)
{
	bool success = false;
	struct sl_element * current;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);

	linked_list_foreach(list, current) {
		if(memcmp(current->data, data, DS_DATA_SIZE(list)) == 0) {
			success = true;
			break;
		}
	}

	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return success;
}

void sl_push_head(single_list list, const void * data)
{
	struct sl_element * current;

	current = __create_element(data, DS_DATA_SIZE(list));

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	__push_head(list, current);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);
}

void sl_push_tail(single_list list, const void * data)
{
	struct sl_element * current;

	current = __create_element(data, DS_DATA_SIZE(list));

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	__push_tail(list, current);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);
}

void * sl_pop_head(single_list list)
{
	void * data = NULL;
	struct sl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	current = __pop_head(list);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

void * sl_pop_tail(single_list list)
{
	void * data = NULL;
	struct sl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	current = __pop_tail(list);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

bool sl_insert(single_list list, const void * data, const size_t pos)
{
	bool success;
	struct sl_element * current;

	current = __create_element(data, DS_DATA_SIZE(list));

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	success = __insert(list, current, pos);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	return success;
}

bool sl_delete(single_list list, const size_t pos)
{
	struct sl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	current = __remove_element(list, pos);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	if(current) {
		free(current->data);
		free(current);

		return true;
	}

	return false;
}

void * sl_remove(single_list list, const size_t pos)
{
	void * data = NULL;
	struct sl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	current = __remove_element(list, pos);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

void * sl_fetch(single_list list, const size_t pos)
{
	struct sl_element * current;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);
	current = __lookup(list, pos);
	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	if(current)
		return current->data;

	return NULL;
}

void sl_reverse(single_list list)
{
	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	__reverse(list);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);
}

void sl_map(single_list list, const map_fn fn)
{
	void * result;
	struct sl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	linked_list_foreach(list, current) {
		result = fn(current->data);

		/* Check if the user allocated a new variable.
		 * If so, it's value must be copied into current->data so that
		 * the newly allocated variable can be freed before it's
		 * reference is lost in the next iteration.
		 */
		if(result != current->data) {
			memcpy(current->data, result, DS_DATA_SIZE(list));
			free(result);
		}
	}
	rwlock_writer_exit(DS_PRIV(list)->rwlock);
}

void * sl_foldr(const single_list list, const foldr_fn fn, const void * init)
{
	void * result;
	void * accumulator;
	struct sl_element * current;

	accumulator = malloc(DS_DATA_SIZE(list));
	memcpy(accumulator, init, DS_DATA_SIZE(list));

	rwlock_reader_entry(DS_PRIV(list)->rwlock);
	linked_list_foreach(list, current) {
		result = fn(current->data, accumulator);

		/* Check if the user allocated a new variable.
		 * If so, it's value must be copied into the accumulator so that
		 * the newly allocated variable can be freed before it's
		 * reference is lost in the next iteration.
		 */
		if(result != accumulator) {
			memcpy(accumulator, result, DS_DATA_SIZE(list));
			free(result);
		}
	}
	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return accumulator;
}

void * sl_foldl(const single_list list, const foldl_fn fn, const void * init)
{
	void * result;
	void * accumulator;
	struct sl_element * current;

	accumulator = malloc(DS_DATA_SIZE(list));
	memcpy(accumulator, init, DS_DATA_SIZE(list));

	rwlock_reader_entry(DS_PRIV(list)->rwlock);
	linked_list_foreach(list, current) {
		result = fn(accumulator, current->data);

		/* Check if the user allocated a new variable.
		 * If so, it's value must be copied into the accumulator so that
		 * the newly allocated variable can be freed before it's
		 * reference is lost in the next iteration.
		 */
		if(result != accumulator) {
			memcpy(accumulator, result, DS_DATA_SIZE(list));
			free(result);
		}
	}
	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return accumulator;
}

bool sl_any(single_list list, const pred_fn pred)
{
	bool success = false;
	struct sl_element * current;

	if(sl_empty(list))
		return false;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);

	linked_list_foreach(list, current) {
		if(pred(current->data)) {
			success = true;
			break;
		}
	}

	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return success;
}

bool sl_all(single_list list, const pred_fn pred)
{
	bool success = true;
	struct sl_element * current;

	if(sl_empty(list))
		return false;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);

	linked_list_foreach(list, current) {
		if(!pred(current->data)) {
			success = false;
			break;
		}
	}

	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return success;
}

bool sl_filter(single_list list, const pred_fn pred)
{
	bool changed = false;
	struct sl_element * current;

	if(sl_empty(list))
		return false;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);

	linked_list_foreach_safe(list, current) {
		if(!pred(current->data)) {
			changed = true;

			__delete_element(list, current);
			free(current->data);
			free(current);
		}
	}

	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	return changed;
}

bool sl_drop_while(single_list list, const pred_fn pred)
{
	size_t orig_length;
	struct sl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);

	orig_length = DS_PRIV(list)->length;

	/* Iterate over the list until we find the first element that doesn't
	 * satisfy the predicate; delete everything before that element.
	 * Otherwise, if an element that fails to satisfy the predicate is never
	 * found, then the entire list should be dropped. */
	linked_list_foreach(list, current) {
		if(!pred(current->data)) {
			__delete_before(list, current);
			break;
		}
	} otherwise(current) {
		__delete_before(list, NULL);
	}

	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	return (orig_length != DS_PRIV(list)->length);
}

bool sl_take_while(single_list list, const pred_fn pred)
{
	size_t orig_length;
	struct sl_element * current;
	struct sl_element * prev = NULL;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);

	orig_length = DS_PRIV(list)->length;

	/* Iterate over the list until we find the first element that doesn't
	 * satisfy the predicate; delete that element and every one after. */
	linked_list_foreach(list, current) {
		if(!pred(current->data)) {
			__delete_after(list, prev);
			break;
		}

		prev = current;
	}

	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	return (orig_length != DS_PRIV(list)->length);
}

#ifdef DEBUG

#define PUT_HR(char, len)                                      \
		for(size_t i = 0; i < len; i++)                \
			putchar(char);                         \
		putchar('\n')

void sl_element_dump(const single_list list, const struct sl_element * current)
{
	uint8_t * bytes = current->data;
	size_t i;

	printf("Address: %p", (void *) current);
	if(current == DS_PRIV(list)->head)
		printf(" (head)");
	if(current == DS_PRIV(list)->tail)
		printf(" (tail)");

	printf("\nNext:    %p\n", (void *) current->next);
	PUT_HR('-', 64);

	for(i = 0; i < DS_DATA_SIZE(list); i++) {
		printf("%#04x ", bytes[i]);

		/* Print 8 bytes per line. */
		if((i > 0) && aligned(i, 8, 0))
			putchar('\n');
	}

	if(!aligned(i, 8, 0))
		putchar('\n');
}

void sl_dump(const single_list list)
{
	struct sl_element * current;

	PUT_HR('#', 64);
	linked_list_foreach(list, current) {
		sl_element_dump(list, current);
		PUT_HR('#', 64);
	}
}

#endif /* DEBUG */
