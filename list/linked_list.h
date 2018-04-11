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

struct element {
	struct element * next;
	struct element * prev;

	union data data;
};

struct linked_list {
	struct element * head;
	struct element * tail;

	size_t length;
};

#define linklist_foreach(list, current)					\
	for(current = list->head; current; current = current->next)

struct linked_list * linklist_alloc(void);
void linklist_free(struct linked_list * ls);
#endif /* __LINKED_LIST_H */
