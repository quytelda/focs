/* list.h - Linked List Implementation
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

#include "list/linked_list.h"

static struct element * __element_init(struct linked_list * list, void * data)
{
	struct  element * elem;

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

static struct element * __lookup_element(struct linked_list * list, size_t pos)
{
	struct element * current;

	if(pos >= list->length)
		return NULL;

	current = list->head;
	for(size_t i = 0; i < pos; i++)
		current = current->next;
	return current;
}

static void __push_head(struct linked_list * list, struct element * current)
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

static struct element * __pop_head(struct linked_list * list)
{
	struct element * current;

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

static void __push_tail(struct linked_list * list, struct element * current)
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

static struct element * __pop_tail(struct linked_list * list)
{
	struct element * current;

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

static bool __insert_element(struct linked_list * list, struct element * current, size_t pos)
{
	struct element * prev;

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

static struct element * __remove_element(struct linked_list * list, size_t pos)
{
	struct element * current;

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

static void __delete_element(struct linked_list * list, struct element * elem)
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

int linklist_alloc(struct linked_list ** list, size_t data_size)
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

void linklist_free(struct linked_list ** list)
{
	struct element * current;

	rwlock_writer_entry((*list)->rwlock);

	(*list)->length = 0;

	linklist_foreach_safe(*list, current) {
		free(current->data);
		free(current);
	}

	rwlock_writer_exit((*list)->rwlock);
	rwlock_free(&(*list)->rwlock);

	free(*list);
}

bool linklist_null(struct linked_list * list)
{
	bool null;

	rwlock_reader_entry(list->rwlock);
	null = (list->length == 0);
	rwlock_reader_exit(list->rwlock);

	return null;
}

void linklist_push_head(struct linked_list * list, void * data)
{
	struct element * current;

	current = __element_init(list, data);

	rwlock_writer_entry(list->rwlock);
	__push_head(list, current);
	rwlock_writer_exit(list->rwlock);
}

void linklist_push_tail(struct linked_list * list, void * data)
{
	struct element * current;

	current = __element_init(list, data);

	rwlock_writer_entry(list->rwlock);
	__push_tail(list, current);
	rwlock_writer_exit(list->rwlock);
}

void * linklist_pop_head(struct linked_list * list)
{
	void * data = NULL;
	struct element * current;

	rwlock_writer_entry(list->rwlock);
	current = __pop_head(list);
	rwlock_writer_exit(list->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

void * linklist_pop_tail(struct linked_list * list)
{
	void * data = NULL;
	struct element * current;

	rwlock_writer_entry(list->rwlock);
	current = __pop_tail(list);
	rwlock_writer_exit(list->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

bool linklist_insert(struct linked_list * list, void * data, size_t pos)
{
	bool success;
	struct element * current;

	current = __element_init(list, data);

	rwlock_writer_entry(list->rwlock);
	success = __insert_element(list, current, pos);
	rwlock_writer_exit(list->rwlock);

	return success;
}

bool linklist_delete(struct linked_list * list, size_t pos)
{
	struct element * current;

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

void * linklist_remove(struct linked_list * list, size_t pos)
{
	void * data = NULL;
	struct element * current;

	rwlock_writer_entry(list->rwlock);
	current = __remove_element(list, pos);
	rwlock_writer_exit(list->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

void * linklist_fetch(struct linked_list * list, size_t pos)
{
	struct element * current;

	rwlock_reader_entry(list->rwlock);
	current = __lookup_element(list, pos);
	rwlock_reader_exit(list->rwlock);

	if(current)
		return current->data;

	return NULL;
}

/**
 * linklist_contains() - Determines if a list contains a value
 * @list: The list to search
 * @data: The data to search for in the list
 *
 * Determines if @list contains an entry matching @data.
 * The operation compares the contents of the memory pointed to by @data, and
 * not the memory addresses of the data pointers.
 *
 * Return: ``true`` if a matching entry is found, otherwise ``false``
 */
bool linklist_contains(struct linked_list * list, void * data)
{
	bool success = false;
	struct element * current;

	rwlock_reader_entry(list->rwlock);

	linklist_foreach(list, current) {
		if(memcmp(current->data, data, list->data_size) == 0) {
			success = true;
			break;
		}
	}

	rwlock_reader_exit(list->rwlock);

	return success;
}

/**
 * linklist_any() - Determines if any value in a list satisifies some condition
 * @list: A list of values
 * @p: The predicate function (representing a condition to be satisfied).
 *
 * Runtime:
 * * worst: O(n)
 * * average: O(n/2)
 * * best: O(1)
 *
 * Iterate over each value stored in @list, and determine if any of them
 * satisfies @p (e.g. @p returns true when passed that value).  This function
 *
 * Return: ``true`` if there is at least one value that satisfies the predicate.
 * Otherwise, it returns ``false``.
 */
bool linklist_any(struct linked_list * list, pred_fn_t p)
{
	bool success = false;
	struct element * current;

	if(linklist_null(list))
		return false;

	rwlock_reader_entry(list->rwlock);

	linklist_foreach(list, current) {
		if(p(current->data)) {
			success = true;
			break;
		}
	}

	rwlock_reader_exit(list->rwlock);

	return success;
}

/**
 * linklist_all() - Determines if all values in a list satisify some condition
 * @list: A list of values
 * @p: The predicate function (representing a condition to be satisfied).
 *
 * Runtime:
 * * worst: O(n)
 * * average: O(n/2)
 * * best: O(1)
 *
 * Iterate over each value stored in @list, and determine if all of them
 * satisfy @p (e.g. @p returns true when passed that value).
 *
 * Return: ``false`` if there is at least one value that does not satisfy the
 * predicate.  Otherwise, it returns ``true``.
 */
bool linklist_all(struct linked_list * list, pred_fn_t p)
{
	bool success = true;
	struct element * current;

	if(linklist_null(list))
		return false;

	rwlock_reader_entry(list->rwlock);

	linklist_foreach(list, current) {
		if(!p(current->data)) {
			success = false;
			break;
		}
	}

	rwlock_reader_exit(list->rwlock);

	return success;
}

/**
 * linklist_map() - Map a function over a linked list in-place.
 * @list: A list of values
 * @fn: A function that will transform each value in the list
 *
 * A map operation iterates over the provided list (@list) and transforms each
 * data element using the function @fn, replacing the old value with the result
 * of the transformation:
 * for i from 0 to &list->length:
 * 	@list[i] = @fn(@list[i])
 */
void linklist_map(struct linked_list * list, map_fn_t fn)
{
	void * result;
	struct element * current;

	rwlock_writer_entry(list->rwlock);
	linklist_foreach(list, current) {
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

/**
 * linklist_foldr() - Right associative fold for linked lists.
 * @list: A list of values to reduce
 * @fn: A binary function that will sequentially reduce values
 * @init: An initial value for the fold
 *
 * A right associative fold uses the binary function @fn to sequentially reduce
 * a list of values to a single value, starting from some initial value @init:
 * @fn(@init, @fn(@list[0], fn(@list[1], ...)))
 *
 * If @list is empty, the fold will be equal to the value of @init.
 */
void * linklist_foldr(const struct linked_list * list,
		      foldr_fn_t fn,
		      const void * init)
{
	void * result;
	void * accumulator;
	struct element * current;

	accumulator = malloc(list->data_size);
	memcpy(accumulator, init, list->data_size);

	rwlock_reader_entry(list->rwlock);
	linklist_foreach(list, current) {
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

/**
 * linklist_foldl() - Left associative fold for linked lists.
 * @list: A list of values to reduce
 * @fn: A binary function that will sequentially reduce values
 * @init: An initial value for the fold
 *
 * A left associative fold uses the binary function @fn to sequentially reduce
 * a list of values to a single value, starting from some initial value @init:
 * @fn(@fn(@fn(..., @init), @list[0]), @list[1])
 *
 * If @list is empty, the fold will be equal to the value of @init.
 */
void * linklist_foldl(const struct linked_list * list,
		      foldl_fn_t fn,
		      const void * init)
{
	void * result;
	void * accumulator;
	struct element * current;

	accumulator = malloc(list->data_size);
	memcpy(accumulator, init, list->data_size);

	rwlock_reader_entry(list->rwlock);
	linklist_foreach(list, current) {
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
