/* single_list.c - Unit Tests for Doubly Linked List
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

#include <check.h>

#include "list/single_list.h"

static const struct ds_properties props = {
	.data_size = sizeof(uint8_t),
};

START_TEST(test_sl_create)
{
	single_list list = NULL;

	list = sl_create(&props);

	ck_assert(list);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_empty_true)
{
	single_list list;

	list = sl_create(&props);
	ck_assert(sl_empty(list));
	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_empty_false)
{
	uint8_t val = 1;
	single_list list;

	list = sl_create(&props);
	sl_push_head(list, &val);
	ck_assert(!sl_empty(list));
	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_push_head_single)
{
	uint8_t val = 1;
	single_list list;

	list = sl_create(&props);
	sl_push_head(list, &val);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);
	ck_assert_int_eq(*(uint8_t *) DS_PRIV(list)->head->data, val);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_push_head_multiple)
{
	uint8_t val1 = 1;
	uint8_t val2 = 2;
	uint8_t val3 = 3;
	single_list list;

	list = sl_create(&props);

	sl_push_head(list, &val1);
	sl_push_head(list, &val2);
	sl_push_head(list, &val3);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 3);

	struct sl_element * current = DS_PRIV(list)->head;
	ck_assert_int_eq(*(uint8_t *) current->data, val3);
	current = current->next;
	ck_assert_int_eq(*(uint8_t *) current->data, val2);
	current = current->next;
	ck_assert_int_eq(*(uint8_t *) current->data, val1);
	ck_assert(!current->next);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_push_tail_single)
{
	uint8_t val = 1;
	single_list list;

	list = sl_create(&props);
	sl_push_tail(list, &val);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);
	ck_assert_int_eq(*(uint8_t *) DS_PRIV(list)->head->data, val);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_push_tail_multiple)
{
	uint8_t val1 = 1;
	uint8_t val2 = 2;
	uint8_t val3 = 3;
	single_list list;

	list = sl_create(&props);

	sl_push_tail(list, &val1);
	sl_push_tail(list, &val2);
	sl_push_tail(list, &val3);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 3);

	struct sl_element * current = DS_PRIV(list)->head;
	ck_assert_int_eq(*(uint8_t *) current->data, val1);
	current = current->next;
	ck_assert_int_eq(*(uint8_t *) current->data, val2);
	current = current->next;
	ck_assert_int_eq(*(uint8_t *) current->data, val3);
	ck_assert(!current->next);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_pop_head_empty)
{
	void * val;
	single_list list;

	list = sl_create(&props);

	val = sl_pop_head(list);

	ck_assert(!val);
	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_pop_head_single)
{
	uint8_t val = 1;
	uint8_t * out;
	single_list list;

	list = sl_create(&props);
	sl_push_head(list, &val);

	out = sl_pop_head(list);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	free(out);
	sl_destroy(&list);

}
END_TEST

START_TEST(test_sl_pop_head_multiple)
{
	uint8_t in1 = 1;
	uint8_t in2 = 2;
	uint8_t in3 = 3;
	uint8_t * out1;
	uint8_t * out2;
	uint8_t * out3;
	single_list list;

	list = sl_create(&props);

	sl_push_head(list, &in1);
	sl_push_head(list, &in2);
	sl_push_head(list, &in3);

	out1 = (uint8_t *) sl_pop_head(list);
	out2 = (uint8_t *) sl_pop_head(list);
	out3 = (uint8_t *) sl_pop_head(list);

	ck_assert(out1);
	ck_assert(out2);
	ck_assert(out3);

	ck_assert_int_eq(*out1, in3);
	ck_assert_int_eq(*out2, in2);
	ck_assert_int_eq(*out3, in1);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	free(out1);
	free(out2);
	free(out3);
	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_pop_tail_empty)
{
	void * val;
	single_list list;

	list = sl_create(&props);

	val = sl_pop_tail(list);

	ck_assert(!val);
	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_pop_tail_single)
{
	uint8_t in = 1;
	uint8_t * out;
	single_list list;
	list = sl_create(&props);
	sl_push_tail(list, &in);

	out = sl_pop_tail(list);

	ck_assert_int_eq(*out, in);
	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	free(out);
	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_pop_tail_multiple)
{
	uint8_t in1 = 1;
	uint8_t in2 = 2;
	uint8_t in3 = 3;
	uint8_t * out1;
	uint8_t * out2;
	uint8_t * out3;
	single_list list;

	list = sl_create(&props);

	sl_push_tail(list, &in1);
	sl_push_tail(list, &in2);
	sl_push_tail(list, &in3);

	out1 = sl_pop_tail(list);
	out2 = sl_pop_tail(list);
	out3 = sl_pop_tail(list);

	ck_assert_int_eq(*out1, in3);
	ck_assert_int_eq(*out2, in2);
	ck_assert_int_eq(*out3, in1);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	free(out1);
	free(out2);
	free(out3);
	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_insert_single)
{
	uint8_t in = 1;
	bool success;
	single_list list;

	list = sl_create(&props);

	success = sl_insert(list, &in, 0);

	ck_assert(success);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_insert_multiple)
{
	bool r1;
	bool r2;
	bool r3;
	bool r4;
	uint8_t in1 = 1;
	uint8_t in2 = 2;
	uint8_t in3 = 3;
	uint8_t in4 = 4;
	uint8_t * out1;
	uint8_t * out2;
	uint8_t * out3;
	uint8_t * out4;

	single_list list;

	list = sl_create(&props);

	/* Insert the elements out of order. */
	/* [4 1 2 3] */
	r1 = sl_insert(list, &in1, 0);
	r2 = sl_insert(list, &in3, 1);
	r3 = sl_insert(list, &in2, 1);
	r4 = sl_insert(list, &in4, 0);

	ck_assert(r1);
	ck_assert(r2);
	ck_assert(r3);
	ck_assert(r4);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 4);

	out1 = sl_pop_head(list); /* 4 */
	out2 = sl_pop_head(list); /* 1 */
	out3 = sl_pop_head(list); /* 2 */
	out4 = sl_pop_head(list); /* 3 */

	ck_assert_int_eq(*out1, in4);
	ck_assert_int_eq(*out2, in1);
	ck_assert_int_eq(*out3, in2);
	ck_assert_int_eq(*out4, in3);

	free(out1);
	free(out2);
	free(out3);
	free(out4);
	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_delete_empty)
{
	bool r1;
	bool r2;
	single_list list;

	list = sl_create(&props);

	r1 = sl_delete(list, 0);
	r2 = sl_delete(list, 1);

	ck_assert(!r1);
	ck_assert(!r2);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_delete_single)
{
	uint8_t in = 1;
	bool r1;
	bool r2;
	bool r3;
	single_list list;

	list = sl_create(&props);
	sl_push_head(list, &in);

	r1 = sl_delete(list, 1); /* [1] -> [1] (false) */
	r2 = sl_delete(list, 0); /* [1] -> [] (true) */
	r3 = sl_delete(list, 0); /* [] -> [] (false) */

	ck_assert(!r1);
	ck_assert(r2);
	ck_assert(!r3);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_delete_multiple)
{
	uint8_t in1 = 1;
	uint8_t in2 = 2;
	uint8_t in3 = 3;
	uint8_t in4 = 4;
	bool r1;
	bool r2;
	bool r3;
	bool r4;
	bool r5;
	single_list list;

	/* Create linked list: [1, 2, 3, 4] */
	list = sl_create(&props);
	sl_push_tail(list, &in1);
	sl_push_tail(list, &in2);
	sl_push_tail(list, &in3);
	sl_push_tail(list, &in4);

	/* Drop the elements out of order. */
	/* [1, 2, 3, 4] -> [1, 2, 4] (true) */
	r1 = sl_delete(list, 2);
	/* [1, 2, 4] -> [2, 4] (true) */
	r2 = sl_delete(list, 0);
	/* [2, 4] -> [2] (true) */
	r3 = sl_delete(list, 1);
	/* [2] -> [] (true) */
	r4 = sl_delete(list, 0);
	/* [] -> [] (false) */
	r5 = sl_delete(list, 0);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	ck_assert(r1);
	ck_assert(r2);
	ck_assert(r3);
	ck_assert(r4);
	ck_assert(!r5);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_remove_empty)
{
	void * val1;
	void * val2;
	single_list list;

	list = sl_create(&props);

	val1 = sl_remove(list, 0);
	val2 = sl_remove(list, 1);

	ck_assert(!val1);
	ck_assert(!val2);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_remove_single)
{
	uint8_t in = 1;
	uint8_t * out;
	single_list list;

	list = sl_create(&props);

	sl_push_head(list, &in);
	out = sl_remove(list, 0);

	ck_assert(out);
	ck_assert_int_eq(*out, in);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	free(out);
	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_remove_multiple)
{
	uint8_t in1 = 1;
	uint8_t in2 = 2;
	uint8_t in3 = 3;
	uint8_t in4 = 4;
	uint8_t * out1;
	uint8_t * out2;
	uint8_t * out3;
	uint8_t * out4;
	uint8_t * out5;
	single_list list;

	/* Create linked list: [1, 2, 3, 4] */
	list = sl_create(&props);
	sl_push_tail(list, &in1);
	sl_push_tail(list, &in2);
	sl_push_tail(list, &in3);
	sl_push_tail(list, &in4);

	/* Remove the elements out of order. */
	/* [1, 2, 3, 4] -> [1, 2, 4] (3) */
	out1 = sl_remove(list, 2);
	/* [1, 2, 4] -> [2, 4] (1) */
	out2 = sl_remove(list, 0);
	/* [2, 4] -> [2] (4) */
	out3 = sl_remove(list, 1);
	/* [2] -> [] (2) */
	out4 = sl_remove(list, 0);
	/* [] -> [] (NULL) */
	out5 = sl_remove(list, 0);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	ck_assert(out1);
	ck_assert(out2);
	ck_assert(out3);
	ck_assert(out4);
	ck_assert(!out5);

	ck_assert_int_eq(*out1, in3);
	ck_assert_int_eq(*out2, in1);
	ck_assert_int_eq(*out3, in4);
	ck_assert_int_eq(*out4, in2);

	free(out1);
	free(out2);
	free(out3);
	free(out4);
	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_fetch_empty)
{
	void * val1;
	void * val2;
	single_list list;

	list = sl_create(&props);

	val1 = sl_fetch(list, 0);
	val2 = sl_fetch(list, 1);

	ck_assert(!val1);
	ck_assert(!val2);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_fetch_single)
{
	uint8_t in = 1;
	uint8_t * out;
	single_list list;

	list = sl_create(&props);
	sl_push_head(list, &in);

	out = sl_fetch(list, 0);

	ck_assert(out);
	ck_assert_int_eq(*out, in);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_fetch_multiple)
{
	uint8_t in1 = 1;
	uint8_t in2 = 2;
	uint8_t in3 = 3;
	uint8_t in4 = 4;
	uint8_t * out1;
	uint8_t * out2;
	uint8_t * out3;
	uint8_t * out4;
	uint8_t * out5;
	single_list list;

	list = sl_create(&props);

	/* Create linked list: [1, 2, 3, 4] */
	sl_push_tail(list, &in1);
	sl_push_tail(list, &in2);
	sl_push_tail(list, &in3);
	sl_push_tail(list, &in4);

	/* Fetch the elements out of order. */
	out1 = sl_fetch(list, 2); /* 3 */
	out2 = sl_fetch(list, 0); /* 1 */
	out3 = sl_fetch(list, 1); /* 2 */
	out4 = sl_fetch(list, 3); /* 4 */
	out5 = sl_fetch(list, 4); /* NULL */

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 4);

	ck_assert(out1);
	ck_assert(out2);
	ck_assert(out3);
	ck_assert(out4);
	ck_assert(!out5);

	ck_assert_int_eq(*out1, in3);
	ck_assert_int_eq(*out2, in1);
	ck_assert_int_eq(*out3, in2);
	ck_assert_int_eq(*out4, in4);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_elem_empty)
{
	bool found;
	uint8_t val = 1;
	single_list list;

	list = sl_create(&props);

	found = sl_elem(list, &val);

	ck_assert(!found);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_elem_single)
{
	bool found1;
	bool found2;
	uint8_t in = 1;
	uint8_t val1 = 1;
	uint8_t val2 = 2;
	single_list list;

	list = sl_create(&props);
	sl_push_head(list, &in);

	found1 = sl_elem(list, &val1);
	found2 = sl_elem(list, &val2);

	ck_assert(found1);
	ck_assert(!found2);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_elem_multiple)
{
	bool found[5];
	uint8_t in[] = {1, 2, 3, 4};
	uint8_t val[] = {1, 2, 3, 4, 5};
	single_list list;

	/* Create linked list: [1, 2, 3, 4] */
	list = sl_create(&props);
	sl_push_tail(list, &in[0]);
	sl_push_tail(list, &in[1]);
	sl_push_tail(list, &in[2]);
	sl_push_tail(list, &in[3]);

	/* Check the elements out of order. */
	found[0] = sl_elem(list, &val[2]);
	found[1] = sl_elem(list, &val[0]);
	found[2] = sl_elem(list, &val[1]);
	found[3] = sl_elem(list, &val[3]);
	found[4] = sl_elem(list, &val[4]);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, sizeof(in));

	ck_assert(found[0]);
	ck_assert(found[1]);
	ck_assert(found[2]);
	ck_assert(found[3]);
	ck_assert(!found[4]);

	sl_destroy(&list);
}
END_TEST

bool pred_gte1(uint8_t * n)
{
	return (*n >= 1);
}

bool pred_lte1(uint8_t * n)
{
	return (*n <= 1);
}

START_TEST(test_sl_any_empty)
{
	bool any;
	single_list list;

	list = sl_create(&props);

	any = sl_any(list, (pred_fn) pred_gte1);

	ck_assert(!any);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_any_single)
{
	bool any[2];
	uint8_t in = 0;
	single_list list;

	list = sl_create(&props);
	sl_push_head(list, &in);

	any[0] = sl_any(list, (pred_fn) pred_gte1);
	any[1] = sl_any(list, (pred_fn) pred_lte1);

	ck_assert(!any[0]);
	ck_assert(any[1]);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_any_multiple)
{
	bool any[2];
	uint8_t in[] = {2, 3, 4};
	single_list list;

	/* Create linked list: [1, 2, 3, 4] */
	list = sl_create(&props);
	sl_push_tail(list, &in[0]);
	sl_push_tail(list, &in[1]);
	sl_push_tail(list, &in[2]);

	/* Check the elements out of order. */
	any[0] = sl_any(list, (pred_fn) pred_gte1);
	any[1] = sl_any(list, (pred_fn) pred_lte1);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, sizeof(in));

	ck_assert(any[0]);
	ck_assert(!any[1]);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_all_empty)
{
	bool all;
	single_list list;

	list = sl_create(&props);

	all = sl_all(list, (pred_fn) pred_gte1);

	ck_assert(!all);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_all_single)
{
	bool all[2];
	uint8_t in = 0;
	single_list list;

	list = sl_create(&props);
	sl_push_head(list, &in);

	all[0] = sl_all(list, (pred_fn) pred_gte1);
	all[1] = sl_all(list, (pred_fn) pred_lte1);

	ck_assert(!all[0]);
	ck_assert(all[1]);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_all_multiple)
{
	bool all[2];
	uint8_t in[] = {2, 3, 4};
	single_list list;

	/* Create linked list: [1, 2, 3, 4] */
	list = sl_create(&props);
	sl_push_tail(list, &in[0]);
	sl_push_tail(list, &in[1]);
	sl_push_tail(list, &in[2]);

	/* Check the elements out of order. */
	all[0] = sl_all(list, (pred_fn) pred_gte1);
	all[1] = sl_all(list, (pred_fn) pred_lte1);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, sizeof(in));

	ck_assert(all[0]);
	ck_assert(!all[1]);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_filter_empty)
{
	bool changed;
	single_list list;

	list = sl_create(&props);

	changed = sl_filter(list, (pred_fn) pred_gte1);

	ck_assert(!changed);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_filter_single)
{
	bool changed;
	uint8_t in = 0;
	single_list list;

	list = sl_create(&props);
	sl_push_head(list, &in);

	changed = sl_filter(list, (pred_fn) pred_lte1);
	ck_assert(!changed);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	changed = sl_filter(list, (pred_fn) pred_gte1);
	ck_assert(changed);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_filter_multiple)
{
	bool changed;
	uint8_t in[] = {0, 2, 0, 2};
	single_list list;

	/* Create linked list: [0, 2, 0, 2] */
	list = sl_create(&props);
	sl_push_tail(list, &in[0]);
	sl_push_tail(list, &in[1]);
	sl_push_tail(list, &in[2]);
	sl_push_tail(list, &in[3]);

	/* filter (> 1) [0, 2, 0, 2] -> [0, 0] */
	changed = sl_filter(list, (pred_fn) pred_gte1);

	ck_assert(changed);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 2);

	/* filter (<= 1) [0, 0] -> [] */
	changed = sl_filter(list, (pred_fn) pred_lte1);

	ck_assert(changed);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_drop_while_empty)
{
	bool changed;
	single_list list;

	list = sl_create(&props);

	changed = sl_drop_while(list, (pred_fn) pred_gte1);

	ck_assert(!changed);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_drop_while_single)
{
	bool changed;
	uint8_t in = 0;
	single_list list;

	/* Singleton List: [0] */
	list = sl_create(&props);
	sl_push_head(list, &in);

	/* dropWhile (> 1) [0] -> [0] */
	changed = sl_drop_while(list, (pred_fn) pred_gte1);

	ck_assert(!changed);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	/* dropWhile (<= 1) [0] -> [] */
	changed = sl_drop_while(list, (pred_fn) pred_lte1);

	ck_assert(changed);
	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_drop_while_multiple)
{
	bool changed;
	uint8_t in[6] = {0, 0, 2, 2, 0, 0};
	single_list list;

	/* Create linked list: [0, 0, 2, 2, 0, 0] */
	list = sl_create(&props);
	sl_push_tail(list, &in[0]);
	sl_push_tail(list, &in[1]);
	sl_push_tail(list, &in[2]);
	sl_push_tail(list, &in[3]);
	sl_push_tail(list, &in[4]);
	sl_push_tail(list, &in[5]);

	/* dropWhile (<= 1) [0, 0, 2, 2, 0, 0] -> [2, 2, 0, 0] */
	changed = sl_drop_while(list, (pred_fn) pred_lte1);

	ck_assert(changed);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 4);

	/* dropWhile (> 1) [2, 2, 0, 0] -> [0, 0]*/
	changed = sl_drop_while(list, (pred_fn) pred_gte1);

	ck_assert(changed);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 2);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_take_while_empty)
{
	bool changed;
	single_list list;

	list = sl_create(&props);

	changed = sl_take_while(list, (pred_fn) pred_gte1);

	ck_assert(!changed);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_take_while_single)
{
	bool changed;
	uint8_t in = 0;
	single_list list;

	/* Singleton List: [0] */
	list = sl_create(&props);
	sl_push_head(list, &in);

	/* takeWhile (<= 1) [0] -> [0] */
	changed = sl_take_while(list, (pred_fn) pred_lte1);

	ck_assert(!changed);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	/* takeWhile (> 1) [0] -> [] */
	changed = sl_take_while(list, (pred_fn) pred_gte1);

	ck_assert(changed);
	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_take_while_multiple)
{
	bool changed;
	uint8_t in[] = {1, 0, 2};
	single_list list;

	/* Create linked list: [1, 0, 2] */
	list = sl_create(&props);
	sl_push_tail(list, &in[0]);
	sl_push_tail(list, &in[1]);
	sl_push_tail(list, &in[2]);

	/* takeWhile (<= 1) [1, 0, 2] -> [1, 0] */
	changed = sl_take_while(list, (pred_fn) pred_lte1);

	ck_assert(changed);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 2);

	/* dropWhile (> 1) [1, 0] -> [1]*/
	changed = sl_take_while(list, (pred_fn) pred_gte1);

	ck_assert(changed);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	sl_destroy(&list);
}
END_TEST

void increment(void * data)
{
	(*(uint8_t *) data)++;
}

START_TEST(test_sl_map_empty)
{
	single_list list;

	list = sl_create(&props);

	sl_map(list, increment);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_map_single)
{
	uint8_t in = 1;
	uint8_t * out;
	single_list list;

	/* Create single element list [1] */
	list = sl_create(&props);
	sl_push_head(list, &in);

	/* Map two increment functions over the list. */
	sl_map(list, increment); /* [1] -> [2] */
	out = sl_fetch(list, 0);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	ck_assert_int_eq(*out, in + 1);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_map_multiple)
{
	uint8_t in1 = 1;
	uint8_t in2 = 2;
	uint8_t in3 = 3;
	uint8_t * out1;
	uint8_t * out2;
	uint8_t * out3;
	single_list list;

	list = sl_create(&props);

	/* list: [1, 2, 3] */
	sl_push_tail(list, &in1);
	sl_push_tail(list, &in2);
	sl_push_tail(list, &in3);

	/* [1, 2, 3] -> [2, 3, 4] */
	sl_map(list, increment);

	out1 = sl_fetch(list, 0);
	out2 = sl_fetch(list, 1);
	out3 = sl_fetch(list, 2);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 3);

	ck_assert_int_eq(*out1, in1 + 1);
	ck_assert_int_eq(*out2, in2 + 1);
	ck_assert_int_eq(*out3, in3 + 1);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_reverse_empty)
{
	single_list list;

	list = sl_create(&props);

	sl_reverse(list);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_reverse_single)
{
	uint8_t in = 1;
	uint8_t * out;
	single_list list;

	list = sl_create(&props);
	sl_push_head(list, &in);

	sl_reverse(list);

	out = sl_fetch(list, 0);
	ck_assert(out);
	ck_assert_int_eq(*out, in);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_reverse_multiple)
{
	uint8_t in[3] = {1, 2, 3};
	uint8_t * out[3];
	single_list list;

	list = sl_create(&props);

	/* Create linked list: [1, 2, 3] */
	sl_push_tail(list, &in[0]);
	sl_push_tail(list, &in[1]);
	sl_push_tail(list, &in[2]);

	/* Reverse the list: [1, 2, 3] -> [3, 2, 1] */
	sl_reverse(list);

	/* Fetch the elements in order. */
	out[0] = sl_fetch(list, 0); /* 3 */
	out[1] = sl_fetch(list, 1); /* 2 */
	out[2] = sl_fetch(list, 2); /* 1 */

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 3);

	ck_assert(out[0]);
	ck_assert(out[1]);
	ck_assert(out[2]);

	ck_assert_int_eq(*out[0], in[2]);
	ck_assert_int_eq(*out[1], in[1]);
	ck_assert_int_eq(*out[2], in[0]);

	sl_destroy(&list);
}
END_TEST

/**
 * substract_right() - Right folding function for testing.
 * @c:   A pointer to a constant byte integer
 * @acc: A pointer to the current fold accumulator
 *
 * Subtract the value of `*acc` from `*c`.  Store the result in `acc`, since we
 * know this is the fold accumulator in right folds.
 */
void subtract_right(const void * c, void * acc)
{
	*(uint8_t *) acc = (*(uint8_t *) c) - (*(uint8_t *) acc);
}

/**
 * subtract_left() - Left folding function for testing.
 * @acc: A pointer to the current fold accumulator
 * @c:   A pointer to a constant byte integer
 *
 * Subtract the value of `*acc` from *c.  Store the result in `acc`, since we
 * know this is the fold accumulator in left folds.
 */
void subtract_left(void * acc, const void * c)
{
	*(uint8_t *) acc = (*(uint8_t *) acc) - (*(uint8_t *) c);
}

START_TEST(test_sl_foldr_empty)
{
	int8_t init = 0;
	int8_t * out;
	single_list list;

	list = sl_create(&props);

	out = sl_foldr(list, subtract_right, &init);

	ck_assert(out);
	ck_assert_int_eq(*out, init);
	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	free(out);
	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_foldr_single)
{
	int8_t in = 1;
	int8_t init = 0;
	int8_t * out;
	single_list list;

	list = sl_create(&props);
	sl_push_head(list, &in);

	out = sl_foldr(list, subtract_right, &init);

	ck_assert(out);
	ck_assert_int_eq(*out, 1);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	free(out);
	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_foldr_multiple)
{
	int8_t in1 = 1;
	int8_t in2 = 2;
	int8_t in3 = 3;
	int8_t init = 0;
	int8_t * out;
	single_list list;

	list = sl_create(&props);

	sl_push_tail(list, &in1);
	sl_push_tail(list, &in2);
	sl_push_tail(list, &in3);

	/* foldr (-) 0 [1, 2, 3] -> 2 */
	out = sl_foldr(list, subtract_right, &init);

	ck_assert(out);
	ck_assert_int_eq(*out, 2);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 3);

	free(out);
	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_foldl_empty)
{
	int8_t init = 0;
	int8_t * out;
	single_list list;

	list = sl_create(&props);

	out = sl_foldl(list, subtract_left, &init);

	ck_assert(out);
	ck_assert_int_eq(*out, init);
	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(sl_empty(list));

	free(out);
	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_foldl_single)
{
	int8_t in = 1;
	int8_t init = 0;
	int8_t * out;
	single_list list;

	list = sl_create(&props);
	sl_push_head(list, &in);

	/* foldl (-) 0 [1] -> -1 */
	out = sl_foldl(list, subtract_left, &init);

	ck_assert(out);
	ck_assert_int_eq(*out, -1);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	free(out);
	sl_destroy(&list);
}
END_TEST

START_TEST(test_sl_foldl_multiple)
{
	int8_t in1 = 1;
	int8_t in2 = 2;
	int8_t in3 = 3;
	int8_t init = 0;
	int8_t * out;
	single_list list;

	list = sl_create(&props);

	sl_push_tail(list, &in1);
	sl_push_tail(list, &in2);
	sl_push_tail(list, &in3);

	/* foldl (-) 0 [1, 2, 3] -> -6 */
	out = sl_foldl(list, subtract_left, &init);

	ck_assert(out);
	ck_assert_int_eq(*out, -6);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 3);

	free(out);
	sl_destroy(&list);
}
END_TEST

Suite * sl_suite(void)
{
	Suite * suite;
	TCase * case_sl_create;
	TCase * case_sl_empty;
	TCase * case_sl_push_head;
	TCase * case_sl_push_tail;
	TCase * case_sl_pop_head;
	TCase * case_sl_pop_tail;
	TCase * case_sl_insert;
	TCase * case_sl_delete;
	TCase * case_sl_remove;
	TCase * case_sl_fetch;
	TCase * case_sl_elem;
	TCase * case_sl_any;
	TCase * case_sl_all;
	TCase * case_sl_filter;
	TCase * case_sl_drop_while;
	TCase * case_sl_take_while;
	TCase * case_sl_map;
	TCase * case_sl_reverse;
	TCase * case_sl_foldl;
	TCase * case_sl_foldr;

	suite = suite_create("Linked List");

	case_sl_create = tcase_create("sl_create");
	case_sl_empty = tcase_create("sl_empty");
	case_sl_push_head = tcase_create("sl_push_head");
	case_sl_push_tail = tcase_create("sl_push_tail");
	case_sl_pop_head = tcase_create("sl_pop_head");
	case_sl_pop_tail = tcase_create("sl_pop_tail");
	case_sl_insert = tcase_create("sl_insert");
	case_sl_delete = tcase_create("sl_delete");
	case_sl_remove = tcase_create("sl_remove");
	case_sl_fetch = tcase_create("sl_fetch");
	case_sl_elem = tcase_create("sl_elem");
	case_sl_any = tcase_create("sl_any");
	case_sl_all = tcase_create("sl_all");
	case_sl_filter = tcase_create("sl_filter");
	case_sl_drop_while = tcase_create("sl_drop_while");
	case_sl_take_while = tcase_create("sl_take_while");
	case_sl_map = tcase_create("sl_map");
	case_sl_reverse = tcase_create("sl_reverse");
	case_sl_foldl = tcase_create("sl_foldl");
	case_sl_foldr = tcase_create("sl_foldr");

	tcase_add_test(case_sl_create, test_sl_create);
	tcase_add_test(case_sl_empty, test_sl_empty_true);
	tcase_add_test(case_sl_empty, test_sl_empty_false);
	tcase_add_test(case_sl_push_head, test_sl_push_head_single);
	tcase_add_test(case_sl_push_head, test_sl_push_head_multiple);
	tcase_add_test(case_sl_push_tail, test_sl_push_tail_single);
	tcase_add_test(case_sl_push_tail, test_sl_push_tail_multiple);
	tcase_add_test(case_sl_pop_head, test_sl_pop_head_empty);
	tcase_add_test(case_sl_pop_head, test_sl_pop_head_single);
	tcase_add_test(case_sl_pop_head, test_sl_pop_head_multiple);
	tcase_add_test(case_sl_pop_tail, test_sl_pop_tail_empty);
	tcase_add_test(case_sl_pop_tail, test_sl_pop_tail_single);
	tcase_add_test(case_sl_pop_tail, test_sl_pop_tail_multiple);
	tcase_add_test(case_sl_insert, test_sl_insert_single);
	tcase_add_test(case_sl_insert, test_sl_insert_multiple);
	tcase_add_test(case_sl_delete, test_sl_delete_empty);
	tcase_add_test(case_sl_delete, test_sl_delete_single);
	tcase_add_test(case_sl_delete, test_sl_delete_multiple);
	tcase_add_test(case_sl_remove, test_sl_remove_empty);
	tcase_add_test(case_sl_remove, test_sl_remove_single);
	tcase_add_test(case_sl_remove, test_sl_remove_multiple);
	tcase_add_test(case_sl_fetch, test_sl_fetch_empty);
	tcase_add_test(case_sl_fetch, test_sl_fetch_single);
	tcase_add_test(case_sl_fetch, test_sl_fetch_multiple);
	tcase_add_test(case_sl_elem, test_sl_elem_empty);
	tcase_add_test(case_sl_elem, test_sl_elem_single);
	tcase_add_test(case_sl_elem, test_sl_elem_multiple);
	tcase_add_test(case_sl_any, test_sl_any_empty);
	tcase_add_test(case_sl_any, test_sl_any_single);
	tcase_add_test(case_sl_any, test_sl_any_multiple);
	tcase_add_test(case_sl_all, test_sl_all_empty);
	tcase_add_test(case_sl_all, test_sl_all_single);
	tcase_add_test(case_sl_all, test_sl_all_multiple);
	tcase_add_test(case_sl_filter, test_sl_filter_empty);
	tcase_add_test(case_sl_filter, test_sl_filter_single);
	tcase_add_test(case_sl_filter, test_sl_filter_multiple);
	tcase_add_test(case_sl_drop_while, test_sl_drop_while_empty);
	tcase_add_test(case_sl_drop_while, test_sl_drop_while_single);
	tcase_add_test(case_sl_drop_while, test_sl_drop_while_multiple);
	tcase_add_test(case_sl_take_while, test_sl_take_while_empty);
	tcase_add_test(case_sl_take_while, test_sl_take_while_single);
	tcase_add_test(case_sl_take_while, test_sl_take_while_multiple);
	tcase_add_test(case_sl_map, test_sl_map_empty);
	tcase_add_test(case_sl_map, test_sl_map_single);
	tcase_add_test(case_sl_map, test_sl_map_multiple);
	tcase_add_test(case_sl_reverse, test_sl_reverse_empty);
	tcase_add_test(case_sl_reverse, test_sl_reverse_single);
	tcase_add_test(case_sl_reverse, test_sl_reverse_multiple);
	tcase_add_test(case_sl_foldr, test_sl_foldr_empty);
	tcase_add_test(case_sl_foldr, test_sl_foldr_single);
	tcase_add_test(case_sl_foldr, test_sl_foldr_multiple);
	tcase_add_test(case_sl_foldl, test_sl_foldl_empty);
	tcase_add_test(case_sl_foldl, test_sl_foldl_single);
	tcase_add_test(case_sl_foldl, test_sl_foldl_multiple);

	suite_add_tcase(suite, case_sl_create);
	suite_add_tcase(suite, case_sl_empty);
	suite_add_tcase(suite, case_sl_push_head);
	suite_add_tcase(suite, case_sl_push_tail);
	suite_add_tcase(suite, case_sl_pop_head);
	suite_add_tcase(suite, case_sl_pop_tail);
	suite_add_tcase(suite, case_sl_insert);
	suite_add_tcase(suite, case_sl_delete);
	suite_add_tcase(suite, case_sl_remove);
	suite_add_tcase(suite, case_sl_fetch);
	suite_add_tcase(suite, case_sl_elem);
	suite_add_tcase(suite, case_sl_any);
	suite_add_tcase(suite, case_sl_all);
	suite_add_tcase(suite, case_sl_filter);
	suite_add_tcase(suite, case_sl_drop_while);
	suite_add_tcase(suite, case_sl_take_while);
	suite_add_tcase(suite, case_sl_map);
	suite_add_tcase(suite, case_sl_reverse);
	suite_add_tcase(suite, case_sl_foldr);
	suite_add_tcase(suite, case_sl_foldl);

	return suite;
}

int main(void)
{
	Suite * suite_dl;
	SRunner * suite_runner;

	suite_dl = sl_suite();

	suite_runner = srunner_create(suite_dl);
	srunner_run_all(suite_runner, CK_NORMAL);
	srunner_free(suite_runner);

	return 0;
}
