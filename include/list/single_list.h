/* single_list.h - Doubly Linked List API
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
#include "focs/data_structure.h"
#include "hof.h"
#include "linked_list.h"
#include "sync/rwlock.h"

/**
 * @struct sl_element
 * Represents an element in a singly linked list.
 *
 * This structure is intended for internal use only.
 */
struct sl_element {
	struct sl_element * next;
	void * data;
};

/**
 * @struct single_list
 * Represents a singly linked list.
 *
 * Initialize this structure with sl_alloc(), and destroy it with sl_destroy().
 */
START_DS(single_list) {
	struct sl_element * head;
	struct sl_element * tail;
	size_t length;

	struct rwlock * rwlock;
} END_DS(single_list);

/**
 * Allocate and initialize a new singly linked list.
 * @param list A pointer to a `struct single_list` pointer.
 * @param data_size The size of data to store in the list
 *
 * Allocates a new singly linked list at the structure pointer pointed to by
 * `list`.
 *
 * @return Upon successful completion, sl_alloc() shall return `0`.  Otherwise,
 * `-1` shall be returned and `errno` set to indicate the error.
 */
single_list sl_create(const struct ds_properties * props);

/**
 * Destroy and deallocate a singly linked list.
 * @param list A pointer to a `struct single_list` pointer.
 * @param data_size The size of data to store in the list
 *
 * De-allocates the singly linked list at the structure pointer pointed to by
 * `list`, as well as de-allocating all data elements contained within `list`.
 */
void sl_destroy(single_list * list);

/**
 * Determine if a list is empty.
 * @param list The list to check
 *
 * @return `true` if `list` is empty, `false` otherwise.
 */
bool sl_empty(single_list list);

/**
 * Determine if a list contains a value.
 * @param list The list to search
 * @param data The data to search for in the list
 *
 * Determines if `list` contains an entry matching `data`.
 * The operation compares the contents of the memory pointed to by `data`, and
 * not the memory addresses of the data pointers.
 *
 * @return `true` if a matching entry is found, otherwise `false`
 */
bool sl_elem(single_list list, void * data);

/**
 * Push a new data element to the head of the list.
 * @param list The list to push onto
 * @param data A pointer to the data to push
 *
 * Push a newly allocated copy of `data` onto the head of `list`.
 */
void sl_push_head(single_list list, void * data);

/**
 * Push a new data element to the tail of the list.
 * @param list The list to push onto
 * @param data A pointer to the data to push
 *
 * Push a newly allocated copy of `data` onto the tail of `list`.
 */
void sl_push_tail(single_list list, void * data);

/**
 * Pop a data element from the head of a list.
 * @param list The list to pop from
 *
 * Remove and return the data element at the head of `list`.  After this
 * operation the returned data element *will no longer be stored in* `list`.
 *
 * @return A pointer to the data element at the head of `list`.  This pointer
 * must be explicitly freed with free() when it is no longer needed.  It is
 * **not** equivalent to the pointer which was used to insert the data into
 * `list`.
 */
void * sl_pop_head(single_list list);

/**
 * Pop a data element from the tail of a list.
 * @param list The list to pop from
 *
 * Remove and return the data element at the tail of `list`.  After this
 * operation the returned data element *will no longer be stored in* `list`.
 *
 * @return A pointer to the data element at the tail of `list`.  This pointer
 * must be explicitly freed with free() when it is no longer needed.  It is
 * **not** equivalent to the pointer which was used to insert the data into
 * `list`.
 */
void * sl_pop_tail(single_list list);

/**
 * Insert a new data element to a given position in a list.
 * @param list The list to inesrt into
 * @param data A pointer to the data to insert
 * @param pos  The position to insert the element at
 *             (must be an index in the range `0..list->length`)
 *
 * Insert a newly allocated copy of `data` into `list` at the index indicated
 * by `pos`.
 *
 * @return `true` if the insertion succeeds, otherwise `false`.
 */
bool sl_insert(single_list list, void * data, size_t pos);

/**
 * Delete a data element from a given position in a list.
 * @param list The list to delete from
 * @param pos  The position to delete the element at
 *             (must be an index in the range `0..list->length - 1`)
 *
 * Delete the copy of `data` stored in `list` at the index indicated by `pos`.
 *
 * @return `true` if the deletion succeeds, otherwise `false`.
 */
bool sl_delete(single_list list, size_t pos);

/**
 * Delete and return a data element from a given position in a list.
 * @param list The list to delete from
 * @param pos  The position to delete the element at
 *             (must be an index in the range `0..list->length - 1`)
 *
 * Delete the reference to `data` stored in `list` at the index indicated by
 * `pos`.
 *
 * @return A pointer to the data removed from `list`, or `NULL` on failure.
 * This pointer must be explicitly freed with free() when it is no longer
 * needed.  It is **not** equivalent to the pointer which was used to insert
 * the data into `list`.
 */
void * sl_remove(single_list list, size_t pos);

/**
 * Fetch a data element from a given position in a list.
 * @param list The list to fetch from
 * @param pos  The index to fetch the element from
 *             (must be an index in the range `0..list->length - 1`)
 *
 * Fetch the data stored at index `pos` in `list`.
 *
 * @return A pointer to the data at index `pos`, or `NULL` on failure.
 * This pointer should **not** be free()d explicitly, or the list will become
 * corrupted.  This pointer will be free()d when sl_destroy() is called, so if
 * the data is needed after the list is destroyed, make a copy of it, or make
 * sure to call sl_remove() on the data's index before destroying the list.
 */
void * sl_fetch(single_list list, size_t pos);

/**
 * Reverse a list in place.
 * @param list The list to reverse
 *
 * Reverses a list in place so that the elements are in reverse order and the
 * head and tail are switched.
 */
void sl_reverse(single_list list);

/* ########################## *
 * # Higher Order Functions # *
 * ########################## */

/**
 * Map a function over a linked list in-place.
 * @param list A list of values
 * @param fn A function that will transform each value in the list
 *
 * A map operation iterates over `list` and transforms each data element using
 * the function `fn`, replacing the old value with the result of the
 * transformation.  In pseudo-code:
 * ```
 * for i from 0 to list->length:
 * 	list[i] = fn(list[i])
 * ```
 */
void sl_map(single_list list, map_fn fn);

/**
 * Right associative fold for singly linked lists.
 * @param list A list of values to reduce
 * @param fn A binary function that will sequentially reduce values
 * @param init An initial value for the fold
 *
 * A right associative fold uses the binary function `fn` to sequentially reduce
 * a list of values to a single value, starting from some initial value `init`:
 * ```
 * fn(init, fn(list[0], fn(list[1], ...)))
 * ```
 *
 * @return The result of a right associate fold over `list`.  If `list` is
 * empty, the fold will be equal to the value of `init`.
 */
void * sl_foldr(const single_list list,
		foldr_fn fn,
		const void * init);

/**
 * Left associative fold for singly linked lists.
 * @param list A list of values to reduce
 * @param fn A binary function that will sequentially reduce values
 * @param init An initial value for the fold
 *
 * A left associative fold uses the binary function `fn` to sequentially reduce
 * a list of values to a single value, starting from some initial value `init`:
 * ```
 * fn(fn(fn(..., init), list[0]), list[1])
 * ```
 *
 * @return The result of a left associate fold over `list`.  If `list` is
 * empty, the fold will be equal to the value of `init`.
 */
void * sl_foldl(const single_list list,
		foldl_fn fn,
		const void * init);

/**
 * Determine if any value in a list satisifies some condition.
 * @param list A list of values
 * @param p The predicate function (representing a condition to be satisfied).
 *
 * Iterate over each value stored in `list`, and determine if any of them
 * satisfies `p` (e.g. `p` returns `true` when passed that value).
 *
 * @return `true` if there is at least one value that satisfies the predicate.
 * Otherwise, it returns `false`.
 */
bool sl_any(single_list list, pred_fn p);

/**
 * Determines if all values in a list satisify some condition
 * @param list A list of values
 * @param p The predicate function (representing a condition to be satisfied).
 *
 * Iterate over each value stored in `list`, and determine if all of them
 * satisfy `p` (e.g. `p` returns true when passed that value).
 *
 * @return `false` if there is at least one value that does not satisfy the
 * predicate.  Otherwise, it returns `true`.
 */
bool sl_all(single_list list, pred_fn p);

/**
 * Filter a list to contain only values that satisfy some predicate.
 * @param list The list to filter
 * @param p The predicate
 *
 * Filter `list` in-place by removing elements that do not satisfy the
 * predicate `p`.  Elements that do satisfy the predicate `p` are not removed
 * from the list.
 */
bool sl_filter(single_list list, pred_fn p);

/**
 * Drop elements from the head of the list until the predicate is unsatisfied.
 *
 * Drop each element that satisfies the predicate `p`, starting at the beginning
 * of `list` and continuing until reaching the first element that does not
 * satisfy the predicate `p`.
 *
 * This function is an in-place equivalent of Haskell's dropWhile.
 */
bool sl_drop_while(single_list list, pred_fn p);

/**
 * Keep elements from the head of the list until the predicate is unsatisfied.
 *
 * Iterate over each element of `list`, starting at the beginning, that
 * satisfies the predicate `p`.  Once an element that does not satisfy the
 * predicate `p` is reached, drop the rest of the list, including that element.
 *
 * This function is an in-place equivalent of Haskell's takeWhile.
 */
bool sl_take_while(single_list list, pred_fn p);

#ifdef GENERICS

static const struct mgmt_operations mgmt_ops = {
	.empty = (empty_mgmt_fn) sl_empty,
	.elem  = (elem_mgmt_fn)  sl_elem,
};

static const struct hof_operations hof_ops = {
	.map        = (map_hof_fn)        sl_map,
	.foldr      = (foldr_hof_fn)      sl_foldr,
	.foldl      = (foldl_hof_fn)      sl_foldl,
	.any        = (any_hof_fn)        sl_any,
	.all        = (all_hof_fn)        sl_all,
	.filter     = (filter_hof_fn)     sl_filter,
	.drop_while = (drop_while_hof_fn) sl_drop_while,
	.take_while = (take_while_hof_fn) sl_take_while,
};

#else /* GENERICS */

static const __unused void * mgmt_ops = NULL;
static const __unused void * hof_ops  = NULL;

#endif /* GENERICS */

#ifdef DEBUG

#include <stdio.h>

void sl_dump(single_list list);

#endif /* DEBUG */

#endif /* __SINGLE_LIST_H */
