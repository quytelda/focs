/* list.h - Linked List Implementation
 * Copyright (C) 2018 Quytelda Kahja
 *
 * This file is part of focs.
 *
 * focs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
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

#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

#include "include/focs.h"
#include "sync/rwlock.h"

struct element {
	struct element * next;
	struct element * prev;

	union data data;
};

struct linked_list {
	struct element * head;
	struct element * tail;
	size_t length;

	struct rwlock rwlock;
};

#define linklist_foreach(list, current)					\
	for(current = list->head; current; current = current->next)

int linklist_alloc(struct linked_list * list);
void linklist_free(struct linked_list * list);

bool linklist_null(struct linked_list * list);
void linklist_push_head(struct linked_list * list, union data data);
void linklist_push_tail(struct linked_list * list, union data data);
union data linklist_pop_head(struct linked_list * list);
union data linklist_pop_tail(struct linked_list * list);
void linklist_insert(struct linked_list * list, union data data, size_t pos);
union data linklist_delete(struct linked_list * list, size_t pos);
union data linklist_fetch(struct linked_list * list, size_t pos);
void linklist_map(struct linked_list * list,
		  union data (* fn)(union data input));
union data linklist_foldr(struct linked_list * list,
			  union data (* fn)(union data a, union data b),
			  union data init);
union data linklist_foldl(struct linked_list * list,
			  union data (* fn)(union data a, union data b),
			  union data init);
#endif /* __LINKED_LIST_H */
