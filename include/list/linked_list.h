/* linked_list.h - Doubly Linked List API
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

#define NEXT_SAFE(current) ((current) ? (current)->next : NULL)

/**
 * Advance through a linked list element by element.
 * @param list The list to iterate over
 * @param current A list element pointer that will point to the current element
 *
 * linked_list_foreach() should be used like a for loop; for example:
 * ```
 * struct element_type * current;
 * linked_list_foreach(list, current) {
 *         do_something(current);
 * }
 * ```
 * Do not modify the element's `next` pointer inside the body of the loop, or
 * the behavior of linked_list_foreach() is undefined.  See linked_list_foreach_safe()
 * instead.
 */
#define linked_list_foreach(list, current)	\
	for(current = DS_PRIV(list)->head;	\
	    current;				\
	    current = current->next)

/**
 * Advance through a linked list element by element with a counter.
 * @param list    The list to iterate over
 * @param current A list element pointer that will point to the current element
 * @param i       The numeric counter to increment on each iteration
 *
 * linked_list_foreach_i() should be used like a for loop; for example:
 * ```
 * int i;
 * struct element_type * current;
 * linked_list_foreach(list, current, i) {
 *         if(some_condition(i)) {
 *                 do_something(current);
 *         }
 * }
 * ```
 *
 * Do not modify the element's `next` pointer inside the body of the loop, or
 * the behavior of linked_list_foreach() is undefined.
 * See linked_list_foreach_i_safe() instead.
 */
#define linked_list_foreach_i(list, current, i)   \
	for(current = DS_PRIV(list)->head, i = 0; \
	    current;                              \
	    current = current->next, i++)

/**
 * Advance through a linked list element by element.
 * @param list The list to iterate over
 * @param current A list element pointer that will point to the current element
 *
 * The syntax of linked_list_foreach_safe() is the same as linked_list_foreach().
 * However, it is safe to change the `next` pointer in the current element
 * during the loop body.
 */
#define linked_list_foreach_safe(list, current)				\
	void * _tmp;							\
	for(current = DS_PRIV(list)->head, _tmp = NEXT_SAFE(current);	\
	    current;							\
	    current = _tmp, _tmp = NEXT_SAFE(current))

/**
 * Advance through a linked list element by element with a counter.
 * @param list    The list to iterate over
 * @param current A list element pointer that will point to the current element
 * @param i       The numeric counter to increment on each iteration
 *
 * The syntax of `linked_list_foreach_safe()` is the same as
 * `linked_list_foreach_i()`.  However, it is safe to change the `next` pointer
 * in the current element during the loop body.
 */
#define linked_list_foreach_i_safe(list, current, i)                         \
	void * _tmp;                                                         \
	for(current = DS_PRIV(list)->head, _tmp = NEXT_SAFE(current), i = 0; \
	    current;                                                         \
	    current = _tmp, _tmp = NEXT_SAFE(current), i++)

/**
 * Advance through a linked list while some condition is true.
 * @param list The list to iterate over
 * @param current A list element pointer that will point to the current element
 * @param condition A condition that will determine whether to continue iterating
 *
 * linked_list_while() should be used like a while loop; for example:
 * ```
 * struct element_type * current;
 * linked_list_while(list, current, current->data != NULL) {
 *         do_something(current->data);
 * }
 * ```
 * Do not modify the element's `next` pointer inside the body of the loop, or
 * the behavior of linked_list_while() is undefined.  See linked_list_while_safe()
 * instead.
 */
#define linked_list_while(list, current, condition)	\
	for(current = DS_PRIV(list)->head;		\
	    current && (condition);			\
	    current = current->next)

/**
 * Advance through a linked list element by element.
 * @param list The list to iterate over
 * @param current A list element pointer that will point to the current element
 * @param condition A condition that will determine whether to continue iterating
 *
 * The syntax of linked_list_while_safe() is the same as linked_list_while().
 * However, it is safe to change the `next` pointer in the current element
 * during the loop body.
 */
#define linked_list_while_safe(list, current, condition)		\
	void * _tmp;							\
	for(current = DS_PRIV(list)->head, _tmp = NEXT_SAFE(current);	\
	    current && (condition);					\
	    current = _tmp, _tmp = NEXT_SAFE(current))

/**
 * Run a block of code if the preceeding loop did not break.
 * @param current The same current element used in the preceeding loop.
 *
 * otherwise() should directly follow a linklist loop in the same way an
 * `else` statement follows an `if` block.  It's function is the same as
 * the `for/else` construction in Python.  If the preceeding loop advanced
 * all the way through the list and did not break or exit the loop on any
 * element, the otherwise block will be called.  For example:
 * ```
 * linked_list_foreach(list, current) {
 *         if(!current->data)
 *                 break;
 * } otherwise(current) {
 *         puts("No NULL data elements.");
 * }
 * ```
 */
#define otherwise(current) if(!current)
