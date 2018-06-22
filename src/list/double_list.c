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

static __pure struct dl_element * __create_element(__immutable(void) data,
	                                           const size_t data_size)
{
	struct dl_element * elem;

	malloc_rof(elem, sizeof(*elem), NULL);
	malloc_gof(elem->data, data_size, exit);

	memcpy(elem->data, data, data_size);
	return elem;

exit:
	free(elem);
	return NULL;
}

static __pure bool __elem(const double_list list, __immutable(void) data)
{
	struct dl_element * current;

	linked_list_foreach(list, current)
		if(DS_DATA_EQ(list, current->data, data))
			return true;

	return false;
}

static __nonulls struct dl_element * __fetch(const double_list list,
	                                     const size_t pos)
{
	struct dl_element * current;

	if(pos >= __LENGTH(list))
		return NULL;

	current = __HEAD(list);
	for(size_t i = 0; i < pos; i++)
		current = current->next; /* `current` shouldn't ever be NULL
		 	                  * in this context unless something
					  * corrupted the list elsewhere. */
	return current;
}

static __nonulls void __push_head(double_list list,
	                          struct dl_element * const current)
{
	current->next = __HEAD(list);
	current->prev = NULL;

	if(__HEAD(list))
		__HEAD(list)->prev = current;
	else
		__TAIL(list) = current;
	__HEAD(list) = current;


	(DS_PRIV(list)->length)++;
}

static __nonulls struct dl_element * __pop_head(double_list list)
{
	struct dl_element * current;

	if(__IS_EMPTY(list))
		return NULL;

	current = __HEAD(list);
	if((__HEAD(list) = current->next))
		__HEAD(list)->prev = NULL;
	else
		__TAIL(list) = NULL;

	(DS_PRIV(list)->length)--;

	return current;
}

static __nonulls void __push_tail(double_list list,
	                          struct dl_element * const current)
{
	current->prev = __TAIL(list);
	current->next = NULL;

	if(__TAIL(list))
		__TAIL(list)->next = current;
	else
		__HEAD(list) = current;
	__TAIL(list) = current;

	(DS_PRIV(list)->length)++;
}

static __nonulls struct dl_element * __pop_tail(double_list list)
{
	struct dl_element * current;

	if(__IS_EMPTY(list))
		return NULL;

	current = __TAIL(list);
	if((__TAIL(list) = current->prev))
		__TAIL(list)->next = NULL;
	else
		__HEAD(list) = NULL;

	(DS_PRIV(list)->length)--;

	return current;
}

static __nonulls bool __insert(double_list list,
	                       struct dl_element * const current,
	                       const size_t pos)
{
	struct dl_element * prev;

	if(pos > DS_PRIV(list)->length)
		return false;

	if(pos == 0) {
		__push_head(list, current);
	} else if(pos == DS_PRIV(list)->length) {
		__push_tail(list, current);
	} else {
		prev = __fetch(list, pos - 1);

		current->prev = prev;
		current->next = prev->next;
		prev->next->prev = current;
		prev->next = current;

		DS_PRIV(list)->length++;
	}

	return true;
}

static __nonulls struct dl_element * __remove(double_list list,
	                                      const size_t pos)
{
	struct dl_element * current;

	if(pos >= DS_PRIV(list)->length)
		return NULL;

	if(pos == 0) {
		current = __pop_head(list);
	} else if(pos == DS_PRIV(list)->length - 1) {
		current = __pop_tail(list);
	} else {
		current = __fetch(list, pos);

		current->prev->next = current->next;
		current->next->prev = current->prev;

		DS_PRIV(list)->length--;
	}

	return current;
}

static __nonulls void __remove_element(double_list list,
	                               const struct dl_element * const elem)
{
	/* Fix head and tail. */
	if(__HEAD(list) == elem)
		__HEAD(list) = elem->next;
	if(__TAIL(list) == elem)
		__TAIL(list) = elem->prev;

	/* Fix pointers in surrounding elements. */
	if(elem->prev)
		elem->prev->next = elem->next;
	if(elem->next)
		elem->next->prev = elem->prev;

	DS_PRIV(list)->length--;
}

static void __delete_before(double_list list, struct dl_element * const mark)
{
	struct dl_element * current;

	linked_list_while_safe(list, current, current != mark) {
		free(current->data);
		free(current);

		DS_PRIV(list)->length--;
	}

	if((__HEAD(list) = mark))
		mark->prev = NULL;
	else
		__TAIL(list) = NULL;
}

static void __delete_after(double_list list, struct dl_element * const mark)
{
	struct dl_element * current;

	double_list_while_rev_safe(list, current, current != mark) {
		free(current->data);
		free(current);

		DS_PRIV(list)->length--;
	}

	if((__TAIL(list) = mark))
		mark->next = NULL;
	else
		__HEAD(list) = NULL;
}

double_list dl_create(__immutable(struct ds_properties) props)
{
	double_list list;
	struct double_list_priv * priv;

	malloc_rof(list, sizeof(*list), NULL);

	DS_INIT(list, props);

	priv = DS_PRIV(list);
	priv->head = NULL;
	priv->tail = NULL;
	priv->length = 0;

        priv->rwlock = rwlock_create();
	if(!priv->rwlock)
		goto exit;

	return list;

exit:
	dl_destroy(&list);
	return NULL;
}

void dl_destroy(double_list * list)
{
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(*list)->rwlock);

	linked_list_foreach_safe(*list, current) {
		free(current->data);
		free(current);
	}

	rwlock_writer_exit(DS_PRIV(*list)->rwlock);
	rwlock_destroy(&DS_PRIV(*list)->rwlock);

	DS_FREE(list);
}

size_t dl_size(const double_list list)
{
	size_t length;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);
	length = __LENGTH(list);
	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return length;
}

bool dl_empty(const double_list list)
{
	bool null;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);
	null = __IS_EMPTY(list);
	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return null;
}

void dl_push_head(double_list list, __immutable(void) data)
{
	struct dl_element * current;

	current = __create_element(data, DS_DATA_SIZE(list));

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	__push_head(list, current);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);
}

void dl_push_tail(double_list list, __immutable(void) data)
{
	struct dl_element * current;

	current = __create_element(data, DS_DATA_SIZE(list));

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	__push_tail(list, current);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);
}

void * dl_pop_head(double_list list)
{
	void * data = NULL;
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	current = __pop_head(list);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

void * dl_pop_tail(double_list list)
{
	void * data = NULL;
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	current = __pop_tail(list);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

bool dl_insert(double_list list, __immutable(void) data, const size_t pos)
{
	bool success;
	struct dl_element * current;

	current = __create_element(data, DS_DATA_SIZE(list));

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	success = __insert(list, current, pos);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	return success;
}

bool dl_delete(double_list list, const size_t pos)
{
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	current = __remove(list, pos);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	if(current) {
		free(current->data);
		free(current);

		return true;
	}

	return false;
}

void * dl_remove(double_list list, const size_t pos)
{
	void * data = NULL;
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	current = __remove(list, pos);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	if(current) {
		data = current->data;
		free(current);
	}

	return data;
}

bool dl_elem(const double_list list, __immutable(void) data)
{
	bool success;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);
	success = __elem(list, data);
	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return success;
}

void * dl_fetch(const double_list list, const size_t pos)
{
	struct dl_element * current;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);
	current = __fetch(list, pos);
	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	if(current)
		return current->data;

	return NULL;
}

void dl_reverse(double_list list)
{
	struct dl_element * current;
	struct dl_element * tmp;

	linked_list_foreach_safe(list, current) {
		tmp = current->prev;
		current->prev = current->next;
		current->next = tmp;
	}

	/* Swap the list head and tail */
	tmp = __HEAD(list);
	__HEAD(list) = __TAIL(list);
	__TAIL(list) = tmp;
}

bool dl_any(const double_list list, const pred_fn p)
{
	bool success = false;
	struct dl_element * current;

	if(dl_empty(list))
		return false;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);

	linked_list_foreach(list, current) {
		if(p(current->data)) {
			success = true;
			break;
		}
	}

	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return success;
}

bool dl_all(const double_list list, const pred_fn p)
{
	bool success = true;
	struct dl_element * current;

	if(dl_empty(list))
		return false;

	rwlock_reader_entry(DS_PRIV(list)->rwlock);

	linked_list_foreach(list, current) {
		if(!p(current->data)) {
			success = false;
			break;
		}
	}

	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return success;
}

bool dl_filter(double_list list, const pred_fn p)
{
	bool changed = false;
	struct dl_element * current;

	if(dl_empty(list))
		return false;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);

	linked_list_foreach_safe(list, current) {
		if(!p(current->data)) {
			changed = true;

			__remove_element(list, current);
			free(current->data);
			free(current);
		}
	}

	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	return changed;
}

bool dl_drop_while(double_list list, const pred_fn p)
{
	size_t orig_length;
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);

	orig_length = DS_PRIV(list)->length;

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

	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	return (orig_length != DS_PRIV(list)->length);
}

bool dl_take_while(double_list list, const pred_fn p)
{
	size_t orig_length;
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);

	orig_length = DS_PRIV(list)->length;

	/* Iterate over the list until we find the first element that doesn't
	 * satisfy the predicate; delete that element and every one after. */
	linked_list_foreach(list, current) {
		if(!p(current->data)) {
			__delete_after(list, current->prev);
			break;
		}
	}

	rwlock_writer_exit(DS_PRIV(list)->rwlock);

	return (orig_length != DS_PRIV(list)->length);
}

void dl_map(double_list list, const map_fn fn)
{
	struct dl_element * current;

	rwlock_writer_entry(DS_PRIV(list)->rwlock);
	linked_list_foreach(list, current)
		fn(current->data);
	rwlock_writer_exit(DS_PRIV(list)->rwlock);
}

void * dl_foldr(const double_list list,
	        const foldr_fn fn,
	        __immutable(void) init)
{
	void * accumulator;
	struct dl_element * current;

	malloc_rof(accumulator, DS_DATA_SIZE(list), NULL);
	memcpy(accumulator, init, DS_DATA_SIZE(list));

	rwlock_reader_entry(DS_PRIV(list)->rwlock);
	linked_list_foreach(list, current)
		fn(current->data, accumulator);
	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return accumulator;
}

void * dl_foldl(const double_list list, const foldl_fn fn, const void * init)
{
	void * accumulator;
	struct dl_element * current;

	malloc_rof(accumulator, DS_DATA_SIZE(list), NULL);
	memcpy(accumulator, init, DS_DATA_SIZE(list));

	rwlock_reader_entry(DS_PRIV(list)->rwlock);
	linked_list_foreach(list, current)
		fn(accumulator, current->data);
	rwlock_reader_exit(DS_PRIV(list)->rwlock);

	return accumulator;
}

#ifdef DEBUG

#define HR_LEN 40
#define BYTES_PER_ROW 8

void __nonulls dl_element_dump(const double_list list,
	                       const struct dl_element * current)
{
	uint8_t * bytes = current->data;
	size_t i;

	printf("Address:  %p", (void *) current);
	if(current == __HEAD(list))
		fputs(" (head)", stdout);
	if(current == __TAIL(list))
		fputs(" (tail)", stdout);

	printf("\nPrevious: %p\nNext:     %p\n",
		(void *) current->prev,
		(void *) current->next);
	PUT_HR('-', HR_LEN);

	for(i = 0; i < DS_DATA_SIZE(list); i++) {
		/* Print BYTES_PER_ROW bytes per line, space seperated. */
		printf("0x%02x ", bytes[i]);

		if((i > 0) && aligned(i, BYTES_PER_ROW, 0))
			putchar('\n');
	}

	if(!aligned(i, BYTES_PER_ROW, 0))
		putchar('\n');
}

void dl_dump(const double_list list)
{
	struct dl_element * current;

	PUT_HR('#', HR_LEN);
	linked_list_foreach(list, current) {
		dl_element_dump(list, current);
		PUT_HR('#', HR_LEN);
	}
}

#endif /* DEBUG */
