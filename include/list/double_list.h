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
#include "sync/rwlock.h"

struct ll_element {
	struct ll_element * next;
	struct ll_element * prev;

	void * data;
};

struct linked_list {
	struct ll_element * head;
	struct ll_element * tail;
	size_t length;
	size_t data_size;

	struct rwlock * rwlock;
};

#define NEXT_SAFE(current) ((current) ? (current)->next : NULL)
#define PREV_SAFE(current) ((current) ? (current)->prev : NULL)

#define linklist_foreach(list, current)					\
	for(current = (list)->head; current; current = current->next)

#define linklist_foreach_rev(list, current)				\
	for(current = (list)->tail; current; current = current->prev)

#define linklist_foreach_safe(list, current)			\
	struct ll_element * _tmp;					\
	for(current = (list)->head, _tmp = NEXT_SAFE(current);	\
	    current;						\
	    current = _tmp, _tmp = NEXT_SAFE(current))

#define linklist_foreach_rev_safe(list, current, _tmp)		\
	struct ll_element * _tmp;					\
	for(current = (list)->tail, _tmp = PREV_SAFE(current);	\
	    current;						\
	    current = _tmp, _tmp = PREV_SAFE(current))

#define linklist_while(list, current, condition)	\
	for(current = (list)->head;			\
	    current && (condition);			\
	    current = current->next)

#define linklist_while_safe(list, current, condition)		\
	struct ll_element * _tmp;					\
	for(current = (list)->head, _tmp = NEXT_SAFE(current);	\
	    current && (condition);				\
	    current = _tmp, _tmp = NEXT_SAFE(current))

#define linklist_while_rev(list, current, condition)	\
	for(current = (list)->tail;			\
	    current && (condition);			\
	    current = current->prev)

#define linklist_while_rev_safe(list, current, condition)	\
	struct ll_element * _tmp;					\
	for(current = (list)->tail, _tmp = PREV_SAFE(current);	\
	    current && (condition);				\
	    current = _tmp, _tmp = PREV_SAFE(current))

#define otherwise(current) if(!current)

/* Creation & Destruction */
int dl_alloc(struct linked_list ** list, size_t data_size);
void dl_free(struct linked_list ** list);

/* Data Management */
void dl_push_head(struct linked_list * list, void * data);
void dl_push_tail(struct linked_list * list, void * data);
void * dl_pop_head(struct linked_list * list);
void * dl_pop_tail(struct linked_list * list);
bool dl_insert(struct linked_list * list, void * data, size_t pos);
bool dl_delete(struct linked_list * list, size_t pos);
void * dl_remove(struct linked_list * list, size_t pos);
void * dl_fetch(struct linked_list * list, size_t pos);

/* Transformations */
void dl_map(struct linked_list * list, map_fn_t fn);
void dl_reverse(struct linked_list * list);
void * dl_foldr(const struct linked_list * list,
		      foldr_fn_t fn,
		      const void * init);
void * dl_foldl(const struct linked_list * list,
		      foldl_fn_t fn,
		      const void * init);

/* Data Properties */
bool dl_null(struct linked_list * list);
bool dl_contains(struct linked_list * list, void * data);
bool dl_any(struct linked_list * list, pred_fn_t p);
bool dl_all(struct linked_list * list, pred_fn_t p);

/* Filtering */
bool dl_filter(struct linked_list * list, pred_fn_t p);
bool dl_drop_while(struct linked_list * list, pred_fn_t p);
bool dl_take_while(struct linked_list * list, pred_fn_t p);

#endif /* __LINKED_LIST_H */
