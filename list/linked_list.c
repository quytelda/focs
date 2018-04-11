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

struct linked_list * linklist_alloc(void)
{
	struct linked_list * ls;

	ls = malloc(sizeof(*ls));
	ls->head = NULL;
	ls->tail = NULL;
	ls->length = 0;

	return ls;
}

void linklist_free(struct linked_list * list)
{
	struct element * current;
	struct element * next;

	list->length = 0;

	for(current = list->head; current; current = next) {
		next = current->next;
		free_null(current);
	}

	free_null(list);
}
