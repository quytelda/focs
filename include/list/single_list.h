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

#ifndef __SINGLE_LIST_H
#define __SINGLE_LIST_H

#include "focs.h"
#include "focs/hof.h"
#include "list/linked_list.h"
#include "sync/rwlock.h"

struct sl_element {
	struct sl_element * next;
	void * data;
};

struct single_list {
	struct sl_element * head;
	struct sl_element * tail;
	size_t length;
	size_t data_size;

	struct rwlock * rwlock;
};

/* Creation & Destruction */
int sl_alloc(struct single_list ** list, size_t data_size);
void sl_free(struct single_list ** list);

/* Data Management */
void sl_push_head(struct single_list * list, void * data);
void sl_push_tail(struct single_list * list, void * data);
void * sl_pop_head(struct single_list * list);
void * sl_pop_tail(struct single_list * list);
bool sl_insert(struct single_list * list, void * data, size_t pos);
bool sl_delete(struct single_list * list, size_t pos);
void * sl_remove(struct single_list * list, size_t pos);
void * sl_fetch(struct single_list * list, size_t pos);

/* Transformations */
void sl_map(struct single_list * list, map_fn fn);
void sl_reverse(struct single_list * list);
void * sl_foldr(const struct single_list * list,
		foldr_fn fn,
		const void * init);
void * sl_foldl(const struct single_list * list,
		foldl_fn fn,
		const void * init);

/* Data Properties */
bool sl_null(struct single_list * list);
bool sl_contains(struct single_list * list, void * data);
bool sl_any(struct single_list * list, pred_fn p);
bool sl_all(struct single_list * list, pred_fn p);

/* Filtering */
bool sl_filter(struct single_list * list, pred_fn p);
bool sl_drop_while(struct single_list * list, pred_fn p);
bool sl_take_while(struct single_list * list, pred_fn p);

#endif /* __SINGLE_LIST_H */
