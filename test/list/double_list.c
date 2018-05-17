/* double_list.c - Unit Tests for Doubly Linked List
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

#include "list/double_list.h"

static const struct ds_properties props = {
	.data_size = sizeof(uint8_t),
};

START_TEST(test_dl_alloc)
{
	double_list list = NULL;

	list = dl_create(&props);

	ck_assert(list);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_null_true)
{
	double_list list;

	list = dl_create(&props);
	ck_assert(dl_null(list));
	dl_free(&list);
}
END_TEST

START_TEST(test_dl_null_false)
{
	uint8_t val = 1;
	double_list list;

	list = dl_create(&props);
	dl_push_head(list, &val);
	ck_assert(!dl_null(list));
	dl_free(&list);
}
END_TEST

START_TEST(test_dl_push_head_single)
{
	uint8_t val = 1;
	double_list list;

	list = dl_create(&props);
	dl_push_head(list, &val);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);
	ck_assert_int_eq(*(uint8_t *) DS_PRIV(list)->head->data, val);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_push_head_multiple)
{
	uint8_t val1 = 1;
	uint8_t val2 = 2;
	uint8_t val3 = 3;
	double_list list;

	list = dl_create(&props);

	dl_push_head(list, &val1);
	dl_push_head(list, &val2);
	dl_push_head(list, &val3);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 3);

	struct dl_element * current = DS_PRIV(list)->head;
	ck_assert_int_eq(*(uint8_t *) current->data, val3);
	current = current->next;
	ck_assert_int_eq(*(uint8_t *) current->data, val2);
	current = current->next;
	ck_assert_int_eq(*(uint8_t *) current->data, val1);
	ck_assert(!current->next);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_push_tail_single)
{
	uint8_t val = 1;
	double_list list;

	list = dl_create(&props);
	dl_push_tail(list, &val);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);
	ck_assert_int_eq(*(uint8_t *) DS_PRIV(list)->head->data, val);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_push_tail_multiple)
{
	uint8_t val1 = 1;
	uint8_t val2 = 2;
	uint8_t val3 = 3;
	double_list list;

	list = dl_create(&props);

	dl_push_tail(list, &val1);
	dl_push_tail(list, &val2);
	dl_push_tail(list, &val3);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 3);

	struct dl_element * current = DS_PRIV(list)->head;
	ck_assert_int_eq(*(uint8_t *) current->data, val1);
	current = current->next;
	ck_assert_int_eq(*(uint8_t *) current->data, val2);
	current = current->next;
	ck_assert_int_eq(*(uint8_t *) current->data, val3);
	ck_assert(!current->next);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_pop_head_empty)
{
	void * val;
	double_list list;

	list = dl_create(&props);

	val = dl_pop_head(list);

	ck_assert(!val);
	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_pop_head_single)
{
	uint8_t val = 1;
	uint8_t * out;
	double_list list;

	list = dl_create(&props);
	dl_push_head(list, &val);

	out = dl_pop_head(list);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	free(out);
	dl_free(&list);

}
END_TEST

START_TEST(test_dl_pop_head_multiple)
{
	uint8_t in1 = 1;
	uint8_t in2 = 2;
	uint8_t in3 = 3;
	uint8_t * out1;
	uint8_t * out2;
	uint8_t * out3;
	double_list list;

	list = dl_create(&props);

	dl_push_head(list, &in1);
	dl_push_head(list, &in2);
	dl_push_head(list, &in3);

	out1 = (uint8_t *) dl_pop_head(list);
	out2 = (uint8_t *) dl_pop_head(list);
	out3 = (uint8_t *) dl_pop_head(list);

	ck_assert(out1);
	ck_assert(out2);
	ck_assert(out3);

	ck_assert_int_eq(*out1, in3);
	ck_assert_int_eq(*out2, in2);
	ck_assert_int_eq(*out3, in1);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	free(out1);
	free(out2);
	free(out3);
	dl_free(&list);
}
END_TEST

START_TEST(test_dl_pop_tail_empty)
{
	void * val;
	double_list list;

	list = dl_create(&props);

	val = dl_pop_tail(list);

	ck_assert(!val);
	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_pop_tail_single)
{
	uint8_t in = 1;
	uint8_t * out;
	double_list list;
	list = dl_create(&props);
	dl_push_tail(list, &in);

	out = dl_pop_tail(list);

	ck_assert_int_eq(*out, in);
	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	free(out);
	dl_free(&list);
}
END_TEST

START_TEST(test_dl_pop_tail_multiple)
{
	uint8_t in1 = 1;
	uint8_t in2 = 2;
	uint8_t in3 = 3;
	uint8_t * out1;
	uint8_t * out2;
	uint8_t * out3;
	double_list list;

	list = dl_create(&props);

	dl_push_tail(list, &in1);
	dl_push_tail(list, &in2);
	dl_push_tail(list, &in3);

	out1 = dl_pop_tail(list);
	out2 = dl_pop_tail(list);
	out3 = dl_pop_tail(list);

	ck_assert_int_eq(*out1, in3);
	ck_assert_int_eq(*out2, in2);
	ck_assert_int_eq(*out3, in1);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	free(out1);
	free(out2);
	free(out3);
	dl_free(&list);
}
END_TEST

START_TEST(test_dl_insert_single)
{
	uint8_t in = 1;
	bool success;
	double_list list;

	list = dl_create(&props);

	success = dl_insert(list, &in, 0);

	ck_assert(success);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_insert_multiple)
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

	double_list list;

	list = dl_create(&props);

	/* Insert the elements out of order. */
	/* [4 1 2 3] */
	r1 = dl_insert(list, &in1, 0);
	r2 = dl_insert(list, &in3, 1);
	r3 = dl_insert(list, &in2, 1);
	r4 = dl_insert(list, &in4, 0);

	ck_assert(r1);
	ck_assert(r2);
	ck_assert(r3);
	ck_assert(r4);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 4);

	out1 = dl_pop_head(list); /* 4 */
	out2 = dl_pop_head(list); /* 1 */
	out3 = dl_pop_head(list); /* 2 */
	out4 = dl_pop_head(list); /* 3 */

	ck_assert_int_eq(*out1, in4);
	ck_assert_int_eq(*out2, in1);
	ck_assert_int_eq(*out3, in2);
	ck_assert_int_eq(*out4, in3);

	free(out1);
	free(out2);
	free(out3);
	free(out4);
	dl_free(&list);
}
END_TEST

START_TEST(test_dl_delete_empty)
{
	bool r1;
	bool r2;
	double_list list;

	list = dl_create(&props);

	r1 = dl_delete(list, 0);
	r2 = dl_delete(list, 1);

	ck_assert(!r1);
	ck_assert(!r2);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_delete_single)
{
	uint8_t in = 1;
	bool r1;
	bool r2;
	bool r3;
	double_list list;

	list = dl_create(&props);
	dl_push_head(list, &in);

	r1 = dl_delete(list, 1); /* [1] -> [1] (false) */
	r2 = dl_delete(list, 0); /* [1] -> [] (true) */
	r3 = dl_delete(list, 0); /* [] -> [] (false) */

	ck_assert(!r1);
	ck_assert(r2);
	ck_assert(!r3);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_delete_multiple)
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
	double_list list;

	/* Create linked list: [1, 2, 3, 4] */
	list = dl_create(&props);
	dl_push_tail(list, &in1);
	dl_push_tail(list, &in2);
	dl_push_tail(list, &in3);
	dl_push_tail(list, &in4);

	/* Drop the elements out of order. */
	/* [1, 2, 3, 4] -> [1, 2, 4] (true) */
	r1 = dl_delete(list, 2);
	/* [1, 2, 4] -> [2, 4] (true) */
	r2 = dl_delete(list, 0);
	/* [2, 4] -> [2] (true) */
	r3 = dl_delete(list, 1);
	/* [2] -> [] (true) */
	r4 = dl_delete(list, 0);
	/* [] -> [] (false) */
	r5 = dl_delete(list, 0);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	ck_assert(r1);
	ck_assert(r2);
	ck_assert(r3);
	ck_assert(r4);
	ck_assert(!r5);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_remove_empty)
{
	void * val1;
	void * val2;
	double_list list;

	list = dl_create(&props);

	val1 = dl_remove(list, 0);
	val2 = dl_remove(list, 1);

	ck_assert(!val1);
	ck_assert(!val2);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_remove_single)
{
	uint8_t in = 1;
	uint8_t * out;
	double_list list;

	list = dl_create(&props);

	dl_push_head(list, &in);
	out = dl_remove(list, 0);

	ck_assert(out);
	ck_assert_int_eq(*out, in);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	free(out);
	dl_free(&list);
}
END_TEST

START_TEST(test_dl_remove_multiple)
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
	double_list list;

	/* Create linked list: [1, 2, 3, 4] */
	list = dl_create(&props);
	dl_push_tail(list, &in1);
	dl_push_tail(list, &in2);
	dl_push_tail(list, &in3);
	dl_push_tail(list, &in4);

	/* Remove the elements out of order. */
	/* [1, 2, 3, 4] -> [1, 2, 4] (3) */
	out1 = dl_remove(list, 2);
	/* [1, 2, 4] -> [2, 4] (1) */
	out2 = dl_remove(list, 0);
	/* [2, 4] -> [2] (4) */
	out3 = dl_remove(list, 1);
	/* [2] -> [] (2) */
	out4 = dl_remove(list, 0);
	/* [] -> [] (NULL) */
	out5 = dl_remove(list, 0);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

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
	dl_free(&list);
}
END_TEST

START_TEST(test_dl_fetch_empty)
{
	void * val1;
	void * val2;
	double_list list;

	list = dl_create(&props);

	val1 = dl_fetch(list, 0);
	val2 = dl_fetch(list, 1);

	ck_assert(!val1);
	ck_assert(!val2);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_fetch_single)
{
	uint8_t in = 1;
	uint8_t * out;
	double_list list;

	list = dl_create(&props);
	dl_push_head(list, &in);

	out = dl_fetch(list, 0);

	ck_assert(out);
	ck_assert_int_eq(*out, in);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_fetch_multiple)
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
	double_list list;

	list = dl_create(&props);

	/* Create linked list: [1, 2, 3, 4] */
	dl_push_tail(list, &in1);
	dl_push_tail(list, &in2);
	dl_push_tail(list, &in3);
	dl_push_tail(list, &in4);

	/* Fetch the elements out of order. */
	out1 = dl_fetch(list, 2); /* 3 */
	out2 = dl_fetch(list, 0); /* 1 */
	out3 = dl_fetch(list, 1); /* 2 */
	out4 = dl_fetch(list, 3); /* 4 */
	out5 = dl_fetch(list, 4); /* NULL */

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

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_contains_empty)
{
	bool found;
	uint8_t val = 1;
	double_list list;

	list = dl_create(&props);

	found = dl_contains(list, &val);

	ck_assert(!found);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_contains_single)
{
	bool found1;
	bool found2;
	uint8_t in = 1;
	uint8_t val1 = 1;
	uint8_t val2 = 2;
	double_list list;

	list = dl_create(&props);
	dl_push_head(list, &in);

	found1 = dl_contains(list, &val1);
	found2 = dl_contains(list, &val2);

	ck_assert(found1);
	ck_assert(!found2);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_contains_multiple)
{
	bool found[5];
	uint8_t in[] = {1, 2, 3, 4};
	uint8_t val[] = {1, 2, 3, 4, 5};
	double_list list;

	/* Create linked list: [1, 2, 3, 4] */
	list = dl_create(&props);
	dl_push_tail(list, &in[0]);
	dl_push_tail(list, &in[1]);
	dl_push_tail(list, &in[2]);
	dl_push_tail(list, &in[3]);

	/* Check the elements out of order. */
	found[0] = dl_contains(list, &val[2]);
	found[1] = dl_contains(list, &val[0]);
	found[2] = dl_contains(list, &val[1]);
	found[3] = dl_contains(list, &val[3]);
	found[4] = dl_contains(list, &val[4]);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, sizeof(in));

	ck_assert(found[0]);
	ck_assert(found[1]);
	ck_assert(found[2]);
	ck_assert(found[3]);
	ck_assert(!found[4]);

	dl_free(&list);
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

START_TEST(test_dl_any_empty)
{
	bool any;
	double_list list;

	list = dl_create(&props);

	any = dl_any(list, (pred_fn) pred_gte1);

	ck_assert(!any);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_any_single)
{
	bool any[2];
	uint8_t in = 0;
	double_list list;

	list = dl_create(&props);
	dl_push_head(list, &in);

	any[0] = dl_any(list, (pred_fn) pred_gte1);
	any[1] = dl_any(list, (pred_fn) pred_lte1);

	ck_assert(!any[0]);
	ck_assert(any[1]);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_any_multiple)
{
	bool any[2];
	uint8_t in[] = {2, 3, 4};
	double_list list;

	/* Create linked list: [1, 2, 3, 4] */
	list = dl_create(&props);
	dl_push_tail(list, &in[0]);
	dl_push_tail(list, &in[1]);
	dl_push_tail(list, &in[2]);

	/* Check the elements out of order. */
	any[0] = dl_any(list, (pred_fn) pred_gte1);
	any[1] = dl_any(list, (pred_fn) pred_lte1);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, sizeof(in));

	ck_assert(any[0]);
	ck_assert(!any[1]);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_all_empty)
{
	bool all;
	double_list list;

	list = dl_create(&props);

	all = dl_all(list, (pred_fn) pred_gte1);

	ck_assert(!all);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_all_single)
{
	bool all[2];
	uint8_t in = 0;
	double_list list;

	list = dl_create(&props);
	dl_push_head(list, &in);

	all[0] = dl_all(list, (pred_fn) pred_gte1);
	all[1] = dl_all(list, (pred_fn) pred_lte1);

	ck_assert(!all[0]);
	ck_assert(all[1]);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_all_multiple)
{
	bool all[2];
	uint8_t in[] = {2, 3, 4};
	double_list list;

	/* Create linked list: [1, 2, 3, 4] */
	list = dl_create(&props);
	dl_push_tail(list, &in[0]);
	dl_push_tail(list, &in[1]);
	dl_push_tail(list, &in[2]);

	/* Check the elements out of order. */
	all[0] = dl_all(list, (pred_fn) pred_gte1);
	all[1] = dl_all(list, (pred_fn) pred_lte1);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, sizeof(in));

	ck_assert(all[0]);
	ck_assert(!all[1]);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_filter_empty)
{
	bool changed;
	double_list list;

	list = dl_create(&props);

	changed = dl_filter(list, (pred_fn) pred_gte1);

	ck_assert(!changed);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_filter_single)
{
	bool changed;
	uint8_t in = 0;
	double_list list;

	list = dl_create(&props);
	dl_push_head(list, &in);

	changed = dl_filter(list, (pred_fn) pred_lte1);
	ck_assert(!changed);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	changed = dl_filter(list, (pred_fn) pred_gte1);
	ck_assert(changed);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_filter_multiple)
{
	bool changed;
	uint8_t in[] = {0, 2, 0, 2};
	double_list list;

	/* Create linked list: [0, 2, 0, 2] */
	list = dl_create(&props);
	dl_push_tail(list, &in[0]);
	dl_push_tail(list, &in[1]);
	dl_push_tail(list, &in[2]);
	dl_push_tail(list, &in[3]);

	/* filter (> 1) [0, 2, 0, 2] -> [0, 0] */
	changed = dl_filter(list, (pred_fn) pred_gte1);

	ck_assert(changed);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 2);

	/* filter (<= 1) [0, 0] -> [] */
	changed = dl_filter(list, (pred_fn) pred_lte1);

	ck_assert(changed);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_drop_while_empty)
{
	bool changed;
	double_list list;

	list = dl_create(&props);

	changed = dl_drop_while(list, (pred_fn) pred_gte1);

	ck_assert(!changed);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_drop_while_single)
{
	bool changed;
	uint8_t in = 0;
	double_list list;

	/* Singleton List: [0] */
	list = dl_create(&props);
	dl_push_head(list, &in);

	/* dropWhile (> 1) [0] -> [0] */
	changed = dl_drop_while(list, (pred_fn) pred_gte1);

	ck_assert(!changed);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	/* dropWhile (<= 1) [0] -> [] */
	changed = dl_drop_while(list, (pred_fn) pred_lte1);

	ck_assert(changed);
	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_drop_while_multiple)
{
	bool changed;
	uint8_t in[6] = {0, 0, 2, 2, 0, 0};
	double_list list;

	/* Create linked list: [0, 0, 2, 2, 0, 0] */
	list = dl_create(&props);
	dl_push_tail(list, &in[0]);
	dl_push_tail(list, &in[1]);
	dl_push_tail(list, &in[2]);
	dl_push_tail(list, &in[3]);
	dl_push_tail(list, &in[4]);
	dl_push_tail(list, &in[5]);

	/* dropWhile (<= 1) [0, 0, 2, 2, 0, 0] -> [2, 2, 0, 0] */
	changed = dl_drop_while(list, (pred_fn) pred_lte1);

	ck_assert(changed);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 4);

	/* dropWhile (> 1) [2, 2, 0, 0] -> [0, 0]*/
	changed = dl_drop_while(list, (pred_fn) pred_gte1);

	ck_assert(changed);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 2);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_take_while_empty)
{
	bool changed;
	double_list list;

	list = dl_create(&props);

	changed = dl_take_while(list, (pred_fn) pred_gte1);

	ck_assert(!changed);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_take_while_single)
{
	bool changed;
	uint8_t in = 0;
	double_list list;

	/* Singleton List: [0] */
	list = dl_create(&props);
	dl_push_head(list, &in);

	/* takeWhile (<= 1) [0] -> [0] */
	changed = dl_take_while(list, (pred_fn) pred_lte1);

	ck_assert(!changed);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	/* takeWhile (> 1) [0] -> [] */
	changed = dl_take_while(list, (pred_fn) pred_gte1);

	ck_assert(changed);
	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_take_while_multiple)
{
	bool changed;
	uint8_t in[] = {1, 0, 2};
	double_list list;

	/* Create linked list: [1, 0, 2] */
	list = dl_create(&props);
	dl_push_tail(list, &in[0]);
	dl_push_tail(list, &in[1]);
	dl_push_tail(list, &in[2]);

	/* takeWhile (<= 1) [1, 0, 2] -> [1, 0] */
	changed = dl_take_while(list, (pred_fn) pred_lte1);

	ck_assert(changed);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 2);

	/* dropWhile (> 1) [1, 0] -> [1]*/
	changed = dl_take_while(list, (pred_fn) pred_gte1);

	ck_assert(changed);
	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	dl_free(&list);
}
END_TEST

uint8_t * map_fn_inplace(uint8_t * data)
{
	(*data)++;
	return data;
}

uint8_t * map_fn_newptr(uint8_t * data)
{
	uint8_t * n;

	n = malloc(sizeof(*data));
	memcpy(n, data, sizeof(*data));
	(*n)++;

	return n;
}

START_TEST(test_dl_map_empty)
{
	double_list list;

	list = dl_create(&props);

	dl_map(list, (map_fn) map_fn_inplace);
	dl_map(list, (map_fn) map_fn_newptr);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_map_single)
{
	uint8_t in = 1;
	uint8_t * out;
	double_list list;

	/* Create single element list [1] */
	list = dl_create(&props);
	dl_push_head(list, &in);

	/* Map two increment functions over the list. */
	dl_map(list, (map_fn) map_fn_inplace); /* [1] -> [2] */
	dl_map(list, (map_fn) map_fn_newptr); /* [2] -> [3] */
	out = dl_fetch(list, 0);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	ck_assert_int_eq(*out, in + 2);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_map_multiple)
{
	uint8_t in1 = 1;
	uint8_t in2 = 2;
	uint8_t in3 = 3;
	uint8_t * out1;
	uint8_t * out2;
	uint8_t * out3;
	double_list list;

	list = dl_create(&props);

	/* list: [1, 2, 3] */
	dl_push_tail(list, &in1);
	dl_push_tail(list, &in2);
	dl_push_tail(list, &in3);

	/* [1, 2, 3] -> [2, 3, 4] */
	dl_map(list, (map_fn) map_fn_inplace);
	/* [2, 3, 4] -> [3, 4, 5] */
	dl_map(list, (map_fn) map_fn_newptr);

	out1 = dl_fetch(list, 0);
	out2 = dl_fetch(list, 1);
	out3 = dl_fetch(list, 2);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 3);

	ck_assert_int_eq(*out1, in1 + 2);
	ck_assert_int_eq(*out2, in2 + 2);
	ck_assert_int_eq(*out3, in3 + 2);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_reverse_empty)
{
	double_list list;

	list = dl_create(&props);

	dl_reverse(list);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_reverse_single)
{
	uint8_t in = 1;
	uint8_t * out;
	double_list list;

	list = dl_create(&props);
	dl_push_head(list, &in);

	dl_reverse(list);

	out = dl_fetch(list, 0);
	ck_assert(out);
	ck_assert_int_eq(*out, in);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	dl_free(&list);
}
END_TEST

START_TEST(test_dl_reverse_multiple)
{
	uint8_t in[3] = {1, 2, 3};
	uint8_t * out[3];
	double_list list;

	list = dl_create(&props);

	/* Create linked list: [1, 2, 3] */
	dl_push_tail(list, &in[0]);
	dl_push_tail(list, &in[1]);
	dl_push_tail(list, &in[2]);

	/* Reverse the list: [1, 2, 3] -> [3, 2, 1] */
	dl_reverse(list);

	/* Fetch the elements in order. */
	out[0] = dl_fetch(list, 0); /* 3 */
	out[1] = dl_fetch(list, 1); /* 2 */
	out[2] = dl_fetch(list, 2); /* 1 */

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 3);

	ck_assert(out[0]);
	ck_assert(out[1]);
	ck_assert(out[2]);

	ck_assert_int_eq(*out[0], in[2]);
	ck_assert_int_eq(*out[1], in[1]);
	ck_assert_int_eq(*out[2], in[0]);

	dl_free(&list);
}
END_TEST

/**
 * foldr_fn_inplace() - Right folding function for testing.
 * @c: A constant byte integer
 * @acc: The byte integer currently in the accumulator
 *
 * Subtract the value of *@acc from *@c.  Store the result in @acc, since we
 * know this is the fold accumulator in right folds.  The data structure will
 * know that since the addresses match, the accumulator has already been
 * updated.
 */
int8_t * foldr_fn_inplace(const int8_t * c, int8_t * acc)
{
	*acc = (*c) - (*acc);
	return acc;
}

/**
 * foldl_fn_inplace() - Left folding function for testing.
 * @acc: The byte integer currently in the accumulator
 * @c: A constant byte integer
 *
 * Subtract the value of *@c from *@acc.  Store the result in @acc, since we
 * know this is the fold accumulator in left folds.  The data structure will
 * know that since the addresses match, the accumulator has already been
 * updated.
 */
int8_t * foldl_fn_inplace(int8_t * acc, const int8_t * c)
{
	*acc = (*acc) - (*c);
	return acc;
}

/**
 * generic_fold_fn() - Generic folding function for testing.
 * @a: A constant byte integer
 * @b: A constant byte integer
 *
 * Subtract the value of *@b from *@a.  Store the result in a new pointer, since
 * we do not know ahead of time whether the function will be used in a left or a
 * right fold.  The data structure will know that since the addresses don't
 * match, the accumulator has not been updated, and it will update it and free
 * our new variable automatically.
 */
int8_t * generic_fold_fn(const int8_t * a, const int8_t * b)
{
	int8_t * n;

	n = malloc(sizeof(n));
	*n = (*a) - (*b);

	return n;
}

START_TEST(test_dl_foldr_empty)
{
	int8_t init = 0;
	int8_t * out1;
	int8_t * out2;
	double_list list;

	list = dl_create(&props);

	out1 = dl_foldr(list, (foldr_fn) foldr_fn_inplace, &init);
	out2 = dl_foldr(list, (foldr_fn) generic_fold_fn, &init);

	ck_assert(out1);
	ck_assert(out2);
	ck_assert_int_eq(*out1, init);
	ck_assert_int_eq(*out2, init);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	free(out1);
	free(out2);
	dl_free(&list);
}
END_TEST

START_TEST(test_dl_foldr_single)
{
	int8_t in = 1;
	int8_t init = 0;
	int8_t * out1;
	int8_t * out2;
	double_list list;

	list = dl_create(&props);
	dl_push_head(list, &in);

	out1 = dl_foldr(list, (foldr_fn) foldr_fn_inplace, &init);
	out2 = dl_foldr(list, (foldr_fn) generic_fold_fn, &init);

	ck_assert(out1);
	ck_assert_int_eq(*out1, 1);
	ck_assert_int_eq(*out2, 1);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	free(out1);
	free(out2);
	dl_free(&list);
}
END_TEST

START_TEST(test_dl_foldr_multiple)
{
	int8_t in1 = 1;
	int8_t in2 = 2;
	int8_t in3 = 3;
	int8_t init = 0;
	int8_t * out1;
	int8_t * out2;
	double_list list;

	list = dl_create(&props);

	dl_push_tail(list, &in1);
	dl_push_tail(list, &in2);
	dl_push_tail(list, &in3);

	/* foldr (-) 0 [1, 2, 3] -> 2 */
	out1 = dl_foldr(list, (foldr_fn) foldr_fn_inplace, &init);
	out2 = dl_foldr(list, (foldr_fn) generic_fold_fn, &init);

	ck_assert(out1);
	ck_assert(out2);
	ck_assert_int_eq(*out1, 2);
	ck_assert_int_eq(*out2, 2);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 3);

	free(out1);
	free(out2);
	dl_free(&list);
}
END_TEST

START_TEST(test_dl_foldl_empty)
{
	int8_t init = 0;
	int8_t * out1;
	int8_t * out2;
	double_list list;

	list = dl_create(&props);

	out1 = dl_foldl(list, (foldl_fn) foldl_fn_inplace, &init);
	out2 = dl_foldl(list, (foldl_fn) generic_fold_fn, &init);

	ck_assert(out1);
	ck_assert(out2);
	ck_assert_int_eq(*out1, init);
	ck_assert_int_eq(*out2, init);

	ck_assert(!DS_PRIV(list)->head);
	ck_assert(!DS_PRIV(list)->tail);
	ck_assert(dl_null(list));

	free(out1);
	free(out2);
	dl_free(&list);
}
END_TEST

START_TEST(test_dl_foldl_single)
{
	int8_t in = 1;
	int8_t init = 0;
	int8_t * out1;
	int8_t * out2;
	double_list list;

	list = dl_create(&props);
	dl_push_head(list, &in);

	/* foldl (-) 0 [1] -> -1 */
	out1 = dl_foldl(list, (foldl_fn) foldl_fn_inplace, &init);
	out2 = dl_foldl(list, (foldl_fn) generic_fold_fn, &init);

	ck_assert(out1);
	ck_assert(out2);
	ck_assert_int_eq(*out1, -1);
	ck_assert_int_eq(*out2, -1);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 1);

	free(out1);
	free(out2);
	dl_free(&list);
}
END_TEST

START_TEST(test_dl_foldl_multiple)
{
	int8_t in1 = 1;
	int8_t in2 = 2;
	int8_t in3 = 3;
	int8_t init = 0;
	int8_t * out1;
	int8_t * out2;
	double_list list;

	list = dl_create(&props);

	dl_push_tail(list, &in1);
	dl_push_tail(list, &in2);
	dl_push_tail(list, &in3);

	/* foldl (-) 0 [1, 2, 3] -> -6 */
	out1 = dl_foldl(list, (foldl_fn) foldl_fn_inplace, &init);
	out2 = dl_foldl(list, (foldl_fn) generic_fold_fn, &init);

	ck_assert(out1);
	ck_assert(out2);
	ck_assert_int_eq(*out1, -6);
	ck_assert_int_eq(*out2, -6);

	ck_assert(DS_PRIV(list)->head);
	ck_assert(DS_PRIV(list)->tail);
	ck_assert_int_eq(DS_PRIV(list)->length, 3);

	free(out1);
	free(out2);
	dl_free(&list);
}
END_TEST

Suite * dl_suite(void)
{
	Suite * suite;
	TCase * case_dl_alloc;
	TCase * case_dl_null;
	TCase * case_dl_push_head;
	TCase * case_dl_push_tail;
	TCase * case_dl_pop_head;
	TCase * case_dl_pop_tail;
	TCase * case_dl_insert;
	TCase * case_dl_delete;
	TCase * case_dl_remove;
	TCase * case_dl_fetch;
	TCase * case_dl_contains;
	TCase * case_dl_any;
	TCase * case_dl_all;
	TCase * case_dl_filter;
	TCase * case_dl_drop_while;
	TCase * case_dl_take_while;
	TCase * case_dl_map;
	TCase * case_dl_reverse;
	TCase * case_dl_foldl;
	TCase * case_dl_foldr;

	suite = suite_create("Linked List");

	case_dl_alloc = tcase_create("dl_alloc");
	case_dl_null = tcase_create("dl_null");
	case_dl_push_head = tcase_create("dl_push_head");
	case_dl_push_tail = tcase_create("dl_push_tail");
	case_dl_pop_head = tcase_create("dl_pop_head");
	case_dl_pop_tail = tcase_create("dl_pop_tail");
	case_dl_insert = tcase_create("dl_insert");
	case_dl_delete = tcase_create("dl_delete");
	case_dl_remove = tcase_create("dl_remove");
	case_dl_fetch = tcase_create("dl_fetch");
	case_dl_contains = tcase_create("dl_contains");
	case_dl_any = tcase_create("dl_any");
	case_dl_all = tcase_create("dl_all");
	case_dl_filter = tcase_create("dl_filter");
	case_dl_drop_while = tcase_create("dl_drop_while");
	case_dl_take_while = tcase_create("dl_take_while");
	case_dl_map = tcase_create("dl_map");
	case_dl_reverse = tcase_create("dl_reverse");
	case_dl_foldl = tcase_create("dl_foldl");
	case_dl_foldr = tcase_create("dl_foldr");

	tcase_add_test(case_dl_alloc, test_dl_alloc);
	tcase_add_test(case_dl_null, test_dl_null_true);
	tcase_add_test(case_dl_null, test_dl_null_false);
	tcase_add_test(case_dl_push_head, test_dl_push_head_single);
	tcase_add_test(case_dl_push_head, test_dl_push_head_multiple);
	tcase_add_test(case_dl_push_tail, test_dl_push_tail_single);
	tcase_add_test(case_dl_push_tail, test_dl_push_tail_multiple);
	tcase_add_test(case_dl_pop_head, test_dl_pop_head_empty);
	tcase_add_test(case_dl_pop_head, test_dl_pop_head_single);
	tcase_add_test(case_dl_pop_head, test_dl_pop_head_multiple);
	tcase_add_test(case_dl_pop_tail, test_dl_pop_tail_empty);
	tcase_add_test(case_dl_pop_tail, test_dl_pop_tail_single);
	tcase_add_test(case_dl_pop_tail, test_dl_pop_tail_multiple);
	tcase_add_test(case_dl_insert, test_dl_insert_single);
	tcase_add_test(case_dl_insert, test_dl_insert_multiple);
	tcase_add_test(case_dl_delete, test_dl_delete_empty);
	tcase_add_test(case_dl_delete, test_dl_delete_single);
	tcase_add_test(case_dl_delete, test_dl_delete_multiple);
	tcase_add_test(case_dl_remove, test_dl_remove_empty);
	tcase_add_test(case_dl_remove, test_dl_remove_single);
	tcase_add_test(case_dl_remove, test_dl_remove_multiple);
	tcase_add_test(case_dl_fetch, test_dl_fetch_empty);
	tcase_add_test(case_dl_fetch, test_dl_fetch_single);
	tcase_add_test(case_dl_fetch, test_dl_fetch_multiple);
	tcase_add_test(case_dl_contains, test_dl_contains_empty);
	tcase_add_test(case_dl_contains, test_dl_contains_single);
	tcase_add_test(case_dl_contains, test_dl_contains_multiple);
	tcase_add_test(case_dl_any, test_dl_any_empty);
	tcase_add_test(case_dl_any, test_dl_any_single);
	tcase_add_test(case_dl_any, test_dl_any_multiple);
	tcase_add_test(case_dl_all, test_dl_all_empty);
	tcase_add_test(case_dl_all, test_dl_all_single);
	tcase_add_test(case_dl_all, test_dl_all_multiple);
	tcase_add_test(case_dl_filter, test_dl_filter_empty);
	tcase_add_test(case_dl_filter, test_dl_filter_single);
	tcase_add_test(case_dl_filter, test_dl_filter_multiple);
	tcase_add_test(case_dl_drop_while, test_dl_drop_while_empty);
	tcase_add_test(case_dl_drop_while, test_dl_drop_while_single);
	tcase_add_test(case_dl_drop_while, test_dl_drop_while_multiple);
	tcase_add_test(case_dl_take_while, test_dl_take_while_empty);
	tcase_add_test(case_dl_take_while, test_dl_take_while_single);
	tcase_add_test(case_dl_take_while, test_dl_take_while_multiple);
	tcase_add_test(case_dl_map, test_dl_map_empty);
	tcase_add_test(case_dl_map, test_dl_map_single);
	tcase_add_test(case_dl_map, test_dl_map_multiple);
	tcase_add_test(case_dl_reverse, test_dl_reverse_empty);
	tcase_add_test(case_dl_reverse, test_dl_reverse_single);
	tcase_add_test(case_dl_reverse, test_dl_reverse_multiple);
	tcase_add_test(case_dl_foldr, test_dl_foldr_empty);
	tcase_add_test(case_dl_foldr, test_dl_foldr_single);
	tcase_add_test(case_dl_foldr, test_dl_foldr_multiple);
	tcase_add_test(case_dl_foldl, test_dl_foldl_empty);
	tcase_add_test(case_dl_foldl, test_dl_foldl_single);
	tcase_add_test(case_dl_foldl, test_dl_foldl_multiple);

	suite_add_tcase(suite, case_dl_alloc);
	suite_add_tcase(suite, case_dl_null);
	suite_add_tcase(suite, case_dl_push_head);
	suite_add_tcase(suite, case_dl_push_tail);
	suite_add_tcase(suite, case_dl_pop_head);
	suite_add_tcase(suite, case_dl_pop_tail);
	suite_add_tcase(suite, case_dl_insert);
	suite_add_tcase(suite, case_dl_delete);
	suite_add_tcase(suite, case_dl_remove);
	suite_add_tcase(suite, case_dl_fetch);
	suite_add_tcase(suite, case_dl_contains);
	suite_add_tcase(suite, case_dl_any);
	suite_add_tcase(suite, case_dl_all);
	suite_add_tcase(suite, case_dl_filter);
	suite_add_tcase(suite, case_dl_drop_while);
	suite_add_tcase(suite, case_dl_take_while);
	suite_add_tcase(suite, case_dl_map);
	suite_add_tcase(suite, case_dl_reverse);
	suite_add_tcase(suite, case_dl_foldr);
	suite_add_tcase(suite, case_dl_foldl);

	return suite;
}

int main(void)
{
	Suite * suite_dl;
	SRunner * suite_runner;

	suite_dl = dl_suite();

	suite_runner = srunner_create(suite_dl);
	srunner_run_all(suite_runner, CK_NORMAL);
	srunner_free(suite_runner);

	return 0;
}
