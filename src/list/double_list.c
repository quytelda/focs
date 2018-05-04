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

static struct dl_element * __element_init(struct double_list * list, void * data)
{
	struct dl_element * elem;

	elem = malloc(sizeof(*elem));
	if(!elem)
		goto exit;

	elem->data = malloc(list->data_size);
	if(!elem->data)
		goto exit;

	memcpy(elem->data, data, list->data_size);

	return elem;

exit:
	if(elem)
		free(elem);

	if(elem->data)
		free(elem->data);

	return NULL;
}

static struct dl_element * __lookup_element(struct double_list * list, size_t pos)
{
	struct dl_element * current;

	if(pos >= list->length)
		return NULL;

	current = list->head;
	for(size_t i = 0; i < pos; i++)
		current = current->next;
	return current;
}

static void __push_head(struct double_list * list, struct dl_element * current)
{
	if(list->head)
		list->head->prev = current;

	current->next = list->head;
	current->prev = NULL;
	list->head = current;

	if(!list->tail)
		list->tail = current;

	(list->length)++;
}

static struct dl_element * __pop_head(struct double_list * list)
{
	struct dl_element * current;

	if(list->length == 0)
		return NULL;

	current = list->head;
	list->head = current->next;

	if(list->head)
		list->head->prev = NULL;
	else
		list->tail = NULL;

	(list->length)--;

	return current;
}

static void __push_tail(struct double_list * list, struct dl_element * current)
{
	if(list->tail)
		list->tail->next = current;

	current->prev = list->tail;
	current->next = NULL;
	list->tail = current;

	if(!list->head)
		list->head = current;

	(list->length)++;
}

static struct dl_element * __pop_tail(struct double_list * list)
{
	struct dl_element * current;

	if(list->length == 0)
		return NULL;

	current = list->tail;
	list->tail = current->prev;

	if(list->tail)
		list->tail->next = NULL;
	else
		list->head = NULL;

	(list->length)--;

	return current;
}

static bool __insert_element(struct double_list * list, struct dl_element * current, size_t pos)
{
	struct dl_element * prev;

	if(pos > list->length)
		return false;

	if(pos == 0) {
		__push_head(list, current);
	} else if(pos == list->length) {
		__push_tail(list, current);
	} else {
		prev = __lookup_element(list, pos - 1);

		current->prev = prev;
		current->next = prev->next;
		prev->next->prev = current;
		prev->next = current;

		(list->length)++;
	}

	return true;
}

static struct dl_element * __remove_element(struct double_list * list, size_t pos)
{
	struct dl_element * current;

	if(pos >= list->length)
		return NULL;

	if(pos == 0) {
		current = __pop_head(list);
	} else if(pos == list->length - 1) {
		current = __pop_tail(list);
	} else {
		current = __lookup_element(list, pos);
		current->prev->next = current->next;
		current->next->prev = current->prev;

		(list->length)--;
	}

	return current;
}

static void __delete_element(struct double_list * list, struct dl_element * elem)
{
	/* Fix head and tail. */
	if(list->head == elem)
		list->head = elem->next;
	if(list->tail == elem)
		list->tail = elem->prev;

	/* Fix pointers in surrounding elements. */
	if(elem->prev)
		elem->prev->next = elem->next;
	if(elem->next)
		elem->next->prev = elem->prev;

	(list->length)--;
}

static void __delete_before(struct double_list * list, struct dl_element * mark)
{
	struct dl_element * current;

	linked_list_while_safe(list, current, current != mark) {
		free(current->data);
		free(current);

		(list->length)--;
	}

	list->head = mark;
	if(mark)
		mark->prev = NULL;
	else
		list->tail = NULL;
}

static void __delete_after(struct double_list * list, struct dl_element * mark)
{
	struct dl_element * current;

	double_list_while_rev_safe(list, current, current != mark) {
		free(current->data);
		free(current);

		(list->length)--;
	}

	list->tail = mark;
	if(mark)
		mark->next = NULL;
	else
		list->head = NULL;
}

int dl_alloc(struct double_list ** list, size_t data_size)
{
	int err;

	*list = calloc(1, sizeof(**list));
	if(!*list) {
		err = -ENOMEM;
		goto exit;
	}

	(*list)->data_size = data_size;
	err = rwlock_alloc(&(*list)->rwlock);
	if(err)
		goto exit;

	return 0;

exit:
	free(*list);
	return err;
}

void dl_free(struct double_list ** list)
{
	struct dl_element * current;

	rwlock_writer_entry((*list)->rwlock);

	(*list)->length = 0;

	linked_list_foreach_safe(*list, current) {
		free(current->data);
		free(current);
	}

	rwlock_writer_exit((*list)->rwlock);
	rwlock_free(&(*list)->rwlock);

	free(*list);
}

bool dl_null(struct double_list * list)
{
	bool null;

	rwlock_reader_entry(list->rwlock);
	null = (list->length == 0);
	rwlock_reader_exit(list->rwlock);

	return null;
}

void dl_push_head(struct double_list * list, void * data)
{
	struct dl_element * current;

	current = __element_init(list, data);

	rwlock_writer_entry(list->rwlock);
	__push_head(list, current);
	rwlock_writer_exit(list->rwlock);
}

void dl_push_tail(struct double_list * list, void * data)
{
	struct dl_element * current;

	current = __element_init(list, data);

	rwlock_writer_entry(list->rwlock);
	__push_tail(list, current);
	rwlock_writer_exit(list->rwlock);
}

void * dl_pop_head(struct double_list * list)
{
	void * data = NULL;
	struct dl_element * current;

	rwlock_writer_entry(list->rwlock);
	current = __pop_head(list);
	rwlock_writer_exit(list->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

void * dl_pop_tail(struct double_list * list)
{
	void * data = NULL;
	struct dl_element * current;

	rwlock_writer_entry(list->rwlock);
	current = __pop_tail(list);
	rwlock_writer_exit(list->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

bool dl_insert(struct double_list * list, void * data, size_t pos)
{
	bool success;
	struct dl_element * current;

	current = __element_init(list, data);

	rwlock_writer_entry(list->rwlock);
	success = __insert_element(list, current, pos);
	rwlock_writer_exit(list->rwlock);

	return success;
}

bool dl_delete(struct double_list * list, size_t pos)
{
	struct dl_element * current;

	rwlock_writer_entry(list->rwlock);
	current = __remove_element(list, pos);
	rwlock_writer_exit(list->rwlock);

	if(current) {
		free(current->data);
		free(current);

		return true;
	}

	return false;
}

void * dl_remove(struct double_list * list, size_t pos)
{
	void * data = NULL;
	struct dl_element * current;

	rwlock_writer_entry(list->rwlock);
	current = __remove_element(list, pos);
	rwlock_writer_exit(list->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

void * dl_fetch(struct double_list * list, size_t pos)
{
	struct dl_element * current;

	rwlock_reader_entry(list->rwlock);
	current = __lookup_element(list, pos);
	rwlock_reader_exit(list->rwlock);

	if(current)
		return current->data;

	return NULL;
}

bool dl_contains(struct double_list * list, void * data)
{
	bool success = false;
	struct dl_element * current;

	rwlock_reader_entry(list->rwlock);

	linked_list_foreach(list, current) {
		if(memcmp(current->data, data, list->data_size) == 0) {
			success = true;
			break;
		}
	}

	rwlock_reader_exit(list->rwlock);

	return success;
}

bool dl_any(struct double_list * list, pred_fn p)
{
	bool success = false;
	struct dl_element * current;

	if(dl_null(list))
		return false;

	rwlock_reader_entry(list->rwlock);

	linked_list_foreach(list, current) {
		if(p(current->data)) {
			success = true;
			break;
		}
	}

	rwlock_reader_exit(list->rwlock);

	return success;
}

bool dl_all(struct double_list * list, pred_fn p)
{
	bool success = true;
	struct dl_element * current;

	if(dl_null(list))
		return false;

	rwlock_reader_entry(list->rwlock);

	linked_list_foreach(list, current) {
		if(!p(current->data)) {
			success = false;
			break;
		}
	}

	rwlock_reader_exit(list->rwlock);

	return success;
}

bool dl_filter(struct double_list * list, pred_fn p)
{
	bool changed = false;
	struct dl_element * current;

	if(dl_null(list))
		return false;

	rwlock_writer_entry(list->rwlock);

	linked_list_foreach_safe(list, current) {
		if(!p(current->data)) {
			changed = true;

			__delete_element(list, current);
			free(current->data);
			free(current);
		}
	}

	rwlock_writer_exit(list->rwlock);

	return changed;
}

bool dl_drop_while(struct double_list * list, pred_fn p)
{
	size_t orig_length;
	struct dl_element * current;

	rwlock_writer_entry(list->rwlock);

	orig_length = list->length;

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

	rwlock_writer_exit(list->rwlock);

	return (orig_length != list->length);
}

bool dl_take_while(struct double_list * list, pred_fn p)
{
	size_t orig_length;
	struct dl_element * current;

	rwlock_writer_entry(list->rwlock);

	orig_length = list->length;

	/* Iterate over the list until we find the first element that doesn't
	 * satisfy the predicate; delete that element and every one after. */
	linked_list_foreach(list, current) {
		if(!p(current->data)) {
			__delete_after(list, current->prev);
			break;
		}
	}

	rwlock_writer_exit(list->rwlock);

	return (orig_length != list->length);
}

void dl_map(struct double_list * list, map_fn fn)
{
	void * result;
	struct dl_element * current;

	rwlock_writer_entry(list->rwlock);
	linked_list_foreach(list, current) {
		result = fn(current->data);

		/* Check if the user allocated a new variable.
		 * If so, it's value must be copied into current->data so that
		 * the newly allocated variable can be freed before it's
		 * reference is lost in the next iteration.
		 */
		if(result != current->data) {
			memcpy(current->data, result, list->data_size);
			free(result);
		}
	}
	rwlock_writer_exit(list->rwlock);
}

void dl_reverse(struct double_list * list)
{
	struct dl_element * current;
	struct dl_element * tmp;

	linked_list_foreach_safe(list, current) {
		tmp = current->prev;
		current->prev = current->next;
		current->next = tmp;
	}

	/* Swap the list head and tail */
	tmp = list->head;
	list->head = list->tail;
	list->tail = tmp;
}

void * dl_foldr(const struct double_list * list,
		      foldr_fn fn,
		      const void * init)
{
	void * result;
	void * accumulator;
	struct dl_element * current;

	accumulator = malloc(list->data_size);
	memcpy(accumulator, init, list->data_size);

	rwlock_reader_entry(list->rwlock);
	linked_list_foreach(list, current) {
		result = fn(current->data, accumulator);

		/* Check if the user allocated a new variable.
		 * If so, it's value must be copied into the accumulator so that
		 * the newly allocated variable can be freed before it's
		 * reference is lost in the next iteration.
		 */
		if(result != accumulator) {
			memcpy(accumulator, result, list->data_size);
			free(result);
		}
	}
	rwlock_reader_exit(list->rwlock);

	return accumulator;
}

void * dl_foldl(const struct double_list * list,
		      foldl_fn fn,
		      const void * init)
{
	void * result;
	void * accumulator;
	struct dl_element * current;

	accumulator = malloc(list->data_size);
	memcpy(accumulator, init, list->data_size);

	rwlock_reader_entry(list->rwlock);
	linked_list_foreach(list, current) {
		result = fn(accumulator, current->data);

		/* Check if the user allocated a new variable.
		 * If so, it's value must be copied into the accumulator so that
		 * the newly allocated variable can be freed before it's
		 * reference is lost in the next iteration.
		 */
		if(result != accumulator) {
			memcpy(accumulator, result, list->data_size);
			free(result);
		}
	}
	rwlock_reader_exit(list->rwlock);

	return accumulator;
}
