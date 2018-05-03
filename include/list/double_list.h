/* double_list.h - Doubly Linked List API
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

#include "focs.h"
#include "list/linked_list.h"
#include "sync/rwlock.h"

struct dl_element {
	struct dl_element * next;
	struct dl_element * prev;

	void * data;
};

struct double_list {
	struct dl_element * head;
	struct dl_element * tail;
	size_t length;
	size_t data_size;

	struct rwlock * rwlock;
};

#define PREV_SAFE(current) ((current) ? (current)->prev : NULL)

#define double_list_foreach_rev(list, current)				\
	for(current = (list)->tail; current; current = current->prev)

#define double_list_foreach_rev_safe(list, current, _tmp)	\
	struct dl_element * _tmp;				\
	for(current = (list)->tail, _tmp = PREV_SAFE(current);	\
	    current;						\
	    current = _tmp, _tmp = PREV_SAFE(current))

#define double_list_while_rev(list, current, condition)	\
	for(current = (list)->tail;			\
	    current && (condition);			\
	    current = current->prev)

#define double_list_while_rev_safe(list, current, condition)	\
	struct dl_element * _tmp;				\
	for(current = (list)->tail, _tmp = PREV_SAFE(current);	\
	    current && (condition);				\
	    current = _tmp, _tmp = PREV_SAFE(current))

/* Creation & Destruction */
int dl_alloc(struct double_list ** list, size_t data_size);
void dl_free(struct double_list ** list);

/* Data Management */
void dl_push_head(struct double_list * list, void * data);
void dl_push_tail(struct double_list * list, void * data);
void * dl_pop_head(struct double_list * list);
void * dl_pop_tail(struct double_list * list);
bool dl_insert(struct double_list * list, void * data, size_t pos);
bool dl_delete(struct double_list * list, size_t pos);
void * dl_remove(struct double_list * list, size_t pos);
void * dl_fetch(struct double_list * list, size_t pos);

/* Transformations */
void dl_map(struct double_list * list, map_fn fn);
void dl_reverse(struct double_list * list);
void * dl_foldr(const struct double_list * list,
		foldr_fn fn,
		const void * init);
void * dl_foldl(const struct double_list * list,
		foldl_fn fn,
		const void * init);

/* Data Properties */
bool dl_null(struct double_list * list);
bool dl_contains(struct double_list * list, void * data);
bool dl_any(struct double_list * list, pred_fn p);
bool dl_all(struct double_list * list, pred_fn p);

/* Filtering */
bool dl_filter(struct double_list * list, pred_fn p);
bool dl_drop_while(struct double_list * list, pred_fn p);
bool dl_take_while(struct double_list * list, pred_fn p);

#endif /* __LINKED_LIST_H */
