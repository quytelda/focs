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

#include "list/double_list.h"

static struct dl_element * __create_element(void * data,
					    size_t data_size)
{
	struct dl_element * elem;

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

static struct dl_element * __lookup_element(double_list list, size_t pos)
{
	struct dl_element * current;

	if(pos >= DS_PRIV(list)->length)
		return NULL;

	current = DS_PRIV(list)->head;
	for(size_t i = 0; i < pos; i++)
		current = current->next;
	return current;
}

static void __push_head(double_list list, struct dl_element * current)
{
	if(DS_PRIV(list)->head)
		DS_PRIV(list)->head->prev = current;

	current->next = DS_PRIV(list)->head;
	current->prev = NULL;
	DS_PRIV(list)->head = current;

	if(!DS_PRIV(list)->tail)
		DS_PRIV(list)->tail = current;

	(DS_PRIV(list)->length)++;
}

static struct dl_element * __pop_head(double_list list)
{
	struct dl_element * current;

	if(DS_PRIV(list)->length == 0)
		return NULL;

	current = DS_PRIV(list)->head;
	DS_PRIV(list)->head = current->next;

	if(DS_PRIV(list)->head)
		DS_PRIV(list)->head->prev = NULL;
	else
		DS_PRIV(list)->tail = NULL;

	(DS_PRIV(list)->length)--;

	return current;
}

static void __push_tail(double_list list, struct dl_element * current)
{
	if(DS_PRIV(list)->tail)
		DS_PRIV(list)->tail->next = current;

	current->prev = DS_PRIV(list)->tail;
	current->next = NULL;
	DS_PRIV(list)->tail = current;

	if(!DS_PRIV(list)->head)
		DS_PRIV(list)->head = current;

	(DS_PRIV(list)->length)++;
}

static struct dl_element * __pop_tail(double_list list)
{
	struct dl_element * current;

	if(DS_PRIV(list)->length == 0)
		return NULL;

	current = DS_PRIV(list)->tail;
	DS_PRIV(list)->tail = current->prev;

	if(DS_PRIV(list)->tail)
		DS_PRIV(list)->tail->next = NULL;
	else
		DS_PRIV(list)->head = NULL;

	(DS_PRIV(list)->length)--;

	return current;
}

static bool __insert_element(double_list list, struct dl_element * current, size_t pos)
{
	struct dl_element * prev;

	if(pos > DS_PRIV(list)->length)
		return false;

	if(pos == 0) {
		__push_head(list, current);
	} else if(pos == DS_PRIV(list)->length) {
		__push_tail(list, current);
	} else {
		prev = __lookup_element(list, pos - 1);

		current->prev = prev;
		current->next = prev->next;
		prev->next->prev = current;
		prev->next = current;

		(DS_PRIV(list)->length)++;
	}

	return true;
}

static struct dl_element * __remove_element(double_list list, size_t pos)
{
	struct dl_element * current;

	if(pos >= DS_PRIV(list)->length)
		return NULL;

	if(pos == 0) {
		current = __pop_head(list);
	} else if(pos == DS_PRIV(list)->length - 1) {
		current = __pop_tail(list);
	} else {
		current = __lookup_element(list, pos);
		current->prev->next = current->next;
		current->next->prev = current->prev;

		(DS_PRIV(list)->length)--;
	}

	return current;
}

static void __delete_element(double_list list, struct dl_element * elem)
{
	/* Fix head and tail. */
	if(DS_PRIV(list)->head == elem)
		DS_PRIV(list)->head = elem->next;
	if(DS_PRIV(list)->tail == elem)
		DS_PRIV(list)->tail = elem->prev;

	/* Fix pointers in surrounding elements. */
	if(elem->prev)
		elem->prev->next = elem->next;
	if(elem->next)
		elem->next->prev = elem->prev;

	(DS_PRIV(list)->length)--;
}

static void __delete_before(double_list list, struct dl_element * mark)
{
	struct dl_element * current;

	linked_list_while_safe(list, current, current != mark) {
		free(current->data);
		free(current);

		(DS_PRIV(list)->length)--;
	}

	DS_PRIV(list)->head = mark;
	if(mark)
		mark->prev = NULL;
	else
		DS_PRIV(list)->tail = NULL;
}

static void __delete_after(double_list list, struct dl_element * mark)
{
	struct dl_element * current;

	double_list_while_rev_safe(list, current, current != mark) {
		free(current->data);
		free(current);

		(DS_PRIV(list)->length)--;
	}

	DS_PRIV(list)->tail = mark;
	if(mark)
		mark->next = NULL;
	else
		DS_PRIV(list)->head = NULL;
}

double_list dl_create(const struct ds_properties * props)
{
	double_list list;
	struct double_list_priv * priv;

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
	dl_free(&list);

	return NULL;
}

void dl_free(double_list * list)
{
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(*list)->rwlock);

	linked_list_foreach_safe(*list, current) {
		free(current->data);
		free(current);
	}

	rwlock_writer_exit(DS_PRIV(*list)->rwlock);
	rwlock_free(&DS_PRIV(*list)->rwlock);

	DS_FREE(list);
}

bool dl_empty(double_list list)
{
	bool null;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);
	null = (DS_PRIV(list)->length == 0);
	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return null;
}

void dl_push_head(double_list list, void * data)
{
	struct dl_element * current;

	current = __create_element(data, DS_DATA_SIZE(list));

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	__push_head(list, current);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);
}

void dl_push_tail(double_list list, void * data)
{
	struct dl_element * current;

	current = __create_element(data, DS_DATA_SIZE(list));

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	__push_tail(list, current);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);
}

void * dl_pop_head(double_list list)
{
	void * data = NULL;
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	current = __pop_head(list);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

void * dl_pop_tail(double_list list)
{
	void * data = NULL;
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	current = __pop_tail(list);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

bool dl_insert(double_list list, void * data, size_t pos)
{
	bool success;
	struct dl_element * current;

	current = __create_element(data, DS_DATA_SIZE(list));

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	success = __insert_element(list, current, pos);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	return success;
}

bool dl_delete(double_list list, size_t pos)
{
	struct dl_element * current;

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

void * dl_remove(double_list list, size_t pos)
{
	void * data = NULL;
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	current = __remove_element(list, pos);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

void * dl_fetch(double_list list, size_t pos)
{
	struct dl_element * current;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);
	current = __lookup_element(list, pos);
	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	if(current)
		return current->data;

	return NULL;
}

bool dl_contains(double_list list, void * data)
{
	bool success = false;
	struct dl_element * current;

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

bool dl_any(double_list list, pred_fn p)
{
	bool success = false;
	struct dl_element * current;

	if(dl_empty(list))
		return false;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);

	linked_list_foreach(list, current) {
		if(p(current->data)) {
			success = true;
			break;
		}
	}

	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return success;
}

bool dl_all(double_list list, pred_fn p)
{
	bool success = true;
	struct dl_element * current;

	if(dl_empty(list))
		return false;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);

	linked_list_foreach(list, current) {
		if(!p(current->data)) {
			success = false;
			break;
		}
	}

	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return success;
}

bool dl_filter(double_list list, pred_fn p)
{
	bool changed = false;
	struct dl_element * current;

	if(dl_empty(list))
		return false;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);

	linked_list_foreach_safe(list, current) {
		if(!p(current->data)) {
			changed = true;

			__delete_element(list, current);
			free(current->data);
			free(current);
		}
	}

	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	return changed;
}

bool dl_drop_while(double_list list, pred_fn p)
{
	size_t orig_length;
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);

	orig_length = DS_PRIV(list)->length;

	/* Iterate over the list until we find the first element that doesn't
	 * satisfy the predicate; delete everything before that element.
	 * Otherwise, if an element that fails to satisfy the predicate is never
	 * found, then the entire list should be dropped. */
	linked_list_foreach(list, current) {
		if(!p(current->data)) {
			__delete_before(list, current);
			break;
		}
	} otherwise(current) {
		__delete_before(list, NULL);
	}

	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	return (orig_length != DS_PRIV(list)->length);
}

bool dl_take_while(double_list list, pred_fn p)
{
	size_t orig_length;
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);

	orig_length = DS_PRIV(list)->length;

	/* Iterate over the list until we find the first element that doesn't
	 * satisfy the predicate; delete that element and every one after. */
	linked_list_foreach(list, current) {
		if(!p(current->data)) {
			__delete_after(list, current->prev);
			break;
		}
	}

	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	return (orig_length != DS_PRIV(list)->length);
}

void dl_map(double_list list, map_fn fn)
{
	void * result;
	struct dl_element * current;

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

void dl_reverse(double_list list)
{
	struct dl_element * current;
	struct dl_element * tmp;

	linked_list_foreach_safe(list, current) {
		tmp = current->prev;
		current->prev = current->next;
		current->next = tmp;
	}

	/* Swap the list head and tail */
	tmp = DS_PRIV(list)->head;
	DS_PRIV(list)->head = DS_PRIV(list)->tail;
	DS_PRIV(list)->tail = tmp;
}

void * dl_foldr(const double_list list,
		      foldr_fn fn,
		      const void * init)
{
	void * result;
	void * accumulator;
	struct dl_element * current;

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

void * dl_foldl(const double_list list,
		      foldl_fn fn,
		      const void * init)
{
	void * result;
	void * accumulator;
	struct dl_element * current;

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
