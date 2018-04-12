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

static void __push_front(struct linked_list * list, struct element * current)
{
	if(list->head)
		list->head->prev = current;

	current->next = list->head;
	current->prev = NULL;
	list->head = current;

	(list->length)++;
}

static struct element * __pop_front(struct linked_list * list)
{
	struct element * current;

	if(!list->head)
		return NULL;

	current = list->head;
	list->head = current->next;
	list->head->prev = NULL;

	(list->length)--;

	return current;
}

static void __push_back(struct linked_list * list, struct element * current)
{
	if(list->tail)
		list->tail->next = current;

	current->prev = list->tail;
	current->next = NULL;
	list->tail = current;

	(list->length)++;
}

static struct element * __pop_back(struct linked_list * list)
{
	struct element * current;

	if(!list->tail)
		return NULL;

	current = list->tail;
	list->tail = current->prev;
	list->tail->next = NULL;

	(list->length)--;

	return current;
}

static bool __insert_element(struct linked_list * list, struct element * current, size_t pos)
{
	struct element * prev;

	if(pos > list->length)
		return false;

	if(pos == 0) {
		__push_front(list, current);
	} else if(pos == list->length) {
		__push_back(list, current);
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
		current = __pop_front(list);
	} else if(pos == list->length) {
		current = __pop_back(list);
	} else {
		current = __lookup_element(list, pos);
		current->prev->next = current->next;
		current->next->prev = current->prev;

		(list->length)--;
	}

	return current;
}

int linklist_alloc(struct linked_list * list)
{
	int err;

	list = calloc(1, sizeof(*list));
	err = rwlock_alloc(&list->rwlock);
	if(err)
		goto exit;

exit:

	free_null(list);
	return err;
}

void linklist_free(struct linked_list * list)
{
	struct element * current;
	struct element * next;

	rwlock_writer_entry(&list->rwlock);

	list->length = 0;

	for(current = list->head; current; current = next) {
		next = current->next;
		free_null(current);
	}

	free_null(list);

	rwlock_writer_exit(&list->rwlock);
}

bool linklist_null(struct linked_list * list)
{
	bool null;

	rwlock_reader_entry(&list->rwlock);
	null = (list->length == 0);
	rwlock_reader_exit(&list->rwlock);

	return null;
}

void linklist_push_head(struct linked_list * list, union data data)
{
	struct element * current;

	current = malloc(sizeof(*current));
	current->data = data;

	rwlock_writer_entry(&list->rwlock);
	__push_front(list, current);
	rwlock_writer_exit(&list->rwlock);
}

void linklist_push_tail(struct linked_list * list, union data data)
{
	struct element * current;

	current = malloc(sizeof(*current));
	current->data = data;

	rwlock_writer_entry(&list->rwlock);
	__push_back(list, current);
	rwlock_writer_exit(&list->rwlock);
}

union data linklist_pop_head(struct linked_list * list)
{
	struct element * current;

	rwlock_writer_entry(&list->rwlock);
	current = __pop_front(list);
	rwlock_writer_exit(&list->rwlock);

	return current->data;
}

union data linklist_pop_tail(struct linked_list * list)
{
	struct element * current;

	rwlock_writer_entry(&list->rwlock);
	current = __pop_back(list);
	rwlock_writer_exit(&list->rwlock);

	return current->data;
}

void linklist_insert(struct linked_list * list, union data data, size_t pos)
{
	struct element * current;

	current = malloc(sizeof(*current));
	current->data = data;

	rwlock_writer_entry(&list->rwlock);
	__insert_element(list, current, pos);
	rwlock_writer_exit(&list->rwlock);
}

union data linklist_delete(struct linked_list * list, size_t pos)
{
	struct element * current;

	rwlock_writer_entry(&list->rwlock);
	current = __delete_element(list, pos);
	rwlock_writer_exit(&list->rwlock);

	return current->data;
}

union data linklist_fetch(struct linked_list * list, size_t pos)
{
	struct element * current;

	rwlock_reader_entry(&list->rwlock);
	current = __lookup_element(list, pos);
	rwlock_reader_exit(&list->rwlock);

	return current->data;
}

void linklist_map(struct linked_list * list,
		  union data (* fn)(union data input))
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
union data linklist_foldr(struct linked_list * list,
			  union data (* fn)(union data a, union data b),
			  union data init)
{
	struct element * current;
	union data sum = init;

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
union data linklist_foldl(struct linked_list * list,
			  union data (* fn)(union data a, union data b),
			  union data init)
{
	struct element * current;
	union data sum = init;

	rwlock_writer_entry(&list->rwlock);
	linklist_foreach(list, current)
		sum = fn(sum, current->data);
	rwlock_writer_exit(&list->rwlock);

	return sum;
}
