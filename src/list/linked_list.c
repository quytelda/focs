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

static struct element * __delete_element(struct linked_list * list, size_t pos)
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
	struct element * next;

	rwlock_writer_entry(&(*list)->rwlock);

	(*list)->length = 0;

	for(current = (*list)->head; current; current = next) {
		next = current->next;
		free(current->data);
		free(current);
	}

	free(*list);

	rwlock_writer_exit(&(*list)->rwlock);
}

bool linklist_null(struct linked_list * list)
{
	bool null;

	rwlock_reader_entry(&list->rwlock);
	null = (list->length == 0);
	rwlock_reader_exit(&list->rwlock);

	return null;
}

void linklist_push_head(struct linked_list * list, void * data)
{
	struct element * current;

	current = __element_init(list, data);

	rwlock_writer_entry(&list->rwlock);
	__push_head(list, current);
	rwlock_writer_exit(&list->rwlock);
}

void linklist_push_tail(struct linked_list * list, void * data)
{
	struct element * current;

	current = __element_init(list, data);

	rwlock_writer_entry(&list->rwlock);
	__push_tail(list, current);
	rwlock_writer_exit(&list->rwlock);
}

void * linklist_pop_head(struct linked_list * list)
{
	struct element * current;

	rwlock_writer_entry(&list->rwlock);
	current = __pop_head(list);
	rwlock_writer_exit(&list->rwlock);

	return current->data;
}

void * linklist_pop_tail(struct linked_list * list)
{
	struct element * current;

	rwlock_writer_entry(&list->rwlock);
	current = __pop_tail(list);
	rwlock_writer_exit(&list->rwlock);

	return current->data;
}

bool linklist_insert(struct linked_list * list, void * data, size_t pos)
{
	bool success;
	struct element * current;

	current = __element_init(list, data);

	rwlock_writer_entry(&list->rwlock);
	success = __insert_element(list, current, pos);
	rwlock_writer_exit(&list->rwlock);

	return success;
}

void * linklist_delete(struct linked_list * list, size_t pos)
{
	struct element * current;

	rwlock_writer_entry(&list->rwlock);
	current = __delete_element(list, pos);
	rwlock_writer_exit(&list->rwlock);

	return current->data;
}

void * linklist_fetch(struct linked_list * list, size_t pos)
{
	struct element * current;

	rwlock_reader_entry(&list->rwlock);
	current = __lookup_element(list, pos);
	rwlock_reader_exit(&list->rwlock);

	return current->data;
}

void linklist_map(struct linked_list * list,
		  void * (* fn)(void * data))
{
	struct element * current;

	rwlock_writer_entry(&list->rwlock);
	linklist_foreach(list, current)
		current->data = fn(current->data);
	rwlock_writer_exit(&list->rwlock);
}

/**
 * linklist_foldr() - Right associative fold for linked lists.
 * fn(list[0], fn(list[1], fn(list[2], ...)))
 */
void * linklist_foldr(struct linked_list * list,
			  void * (* fn)(void * a, void * b),
			  void * init)
{
	struct element * current;
	void * sum = init;

	rwlock_writer_entry(&list->rwlock);
	linklist_foreach(list, current)
		sum = fn(current->data, sum);
	rwlock_writer_exit(&list->rwlock);

	return sum;
}

/**
 * linklist_foldl() - Left associative fold for linked lists.
 * fn(fn(fn(..., list[0]), list[1]), list[2])
 */
void * linklist_foldl(struct linked_list * list,
			  void * (* fn)(void * a, void * b),
			  void * init)
{
	struct element * current;
	void * sum = init;

	rwlock_writer_entry(&list->rwlock);
	linklist_foreach(list, current)
		sum = fn(sum, current->data);
	rwlock_writer_exit(&list->rwlock);

	return sum;
}
