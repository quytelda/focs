/* ring_buffer.c - Unit Tests for the Ring Buffer API
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

#include "list/array.h"
#include "list/ring_buffer.h"

static const struct ds_properties props = {
	.data_size = sizeof(uint8_t), /* Data size is 1B. */
	.entries   = 10,              /* 10 Data Blocks. */
};

static ring_buffer buffer;

void setup(void)
{
	buffer = rb_create(&props);
}

void takedown(void)
{
	rb_destroy(&buffer);
}

/**
 * Test that `buf` has the properties of a newly created ring buffer.
 * `buf` was created during test setup0, and should be in a pristine state.
 */
START_TEST(test_rb_create)
{
	ck_assert(buffer);
	ck_assert(rb_empty(buffer));
	ck_assert(!rb_full(buffer));
}
END_TEST

/**
 * Test that a single element can be pushed to (and subsequently popped from) to
 * the head of `buf`.
 */
START_TEST(test_rb_push_head_single)
{
	uint8_t in = 1;
	uint8_t * out;
	bool ret;

	ret = rb_push_head(buffer, &in);
	out = rb_pop_head(buffer);

	ck_assert(ret);
	ck_assert(out);
	ck_assert_int_eq(*out, in);

	free(out);
}
END_TEST

START_TEST(test_rb_push_head_multiple)
{
	uint8_t in[] = {1, 2, 3};
	uint8_t * out[array_size(in)];
	bool ret[array_size(in)];

	for(size_t i = 0; i < array_size(in); i++)
		ret[i] = rb_push_head(buffer, &in[i]);

	ck_assert_int_eq(rb_size(buffer), array_size(in));

	for(size_t i = 0; i < array_size(in); i++)
		out[i] = rb_pop_tail(buffer);

	for(size_t i = 0; i < array_size(in); i++) {
		ck_assert(ret[i]);
		ck_assert(out[i]);
		ck_assert_int_eq(*out[i], in[i]);

		free(out[i]);
	}
}
END_TEST

START_TEST(test_rb_push_tail_single)
{
	uint8_t in = 1;
	uint8_t * out;
	bool ret;

	ret = rb_push_tail(buffer, &in);
	out = rb_pop_tail(buffer);

	ck_assert(ret);
	ck_assert(out);
	ck_assert_int_eq(*out, in);

	free(out);
}
END_TEST

START_TEST(test_rb_push_tail_multiple)
{
	uint8_t in[] = {1, 2, 3};
	uint8_t * out[array_size(in)];
	bool ret[array_size(in)];

	for(size_t i = 0; i < array_size(in); i++)
		ret[i] = rb_push_tail(buffer, &in[i]);

	ck_assert_int_eq(rb_size(buffer), array_size(in));

	for(size_t i = 0; i < array_size(in); i++)
		out[i] = rb_pop_head(buffer);

	for(size_t i = 0; i < array_size(in); i++) {
		ck_assert(ret[i]);
		ck_assert(out[i]);
		ck_assert_int_eq(*out[i], in[i]);

		free(out[i]);
	}
}
END_TEST

START_TEST(test_rb_pop_head_empty)
{
	ring_buffer buf;
	uint8_t * out;

	buf = rb_create(&props);

	out = rb_pop_head(buf);

	ck_assert(!out);
	ck_assert(rb_empty(buf));

	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_pop_head_single)
{
	uint8_t in = 1;
	uint8_t * out;
	ring_buffer buf = NULL;

	/* Create list: [1] */
	buf = rb_create(&props);
	rb_push_head(buf, &in);

	out = rb_pop_head(buf);

	ck_assert(out);
	ck_assert_int_eq(*out, in);
	ck_assert(rb_empty(buf));

	free(out);
	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_pop_head_multiple)
{
	uint8_t in[] = {1, 2, 3};
	uint8_t * out[3];
	ring_buffer buf = NULL;

	/* Create list: [3, 2, 1] */
	buf = rb_create(&props);
	rb_push_head(buf, &in[0]);
	rb_push_head(buf, &in[1]);
	rb_push_head(buf, &in[2]);

	out[0] = rb_pop_head(buf);
	out[1] = rb_pop_head(buf);
	out[2] = rb_pop_head(buf);

	ck_assert(out[0]);
	ck_assert(out[1]);
	ck_assert(out[2]);

	ck_assert_int_eq(*out[0], in[2]);
	ck_assert_int_eq(*out[1], in[1]);
	ck_assert_int_eq(*out[2], in[0]);

	free(out[0]);
	free(out[1]);
	free(out[2]);
	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_pop_tail_empty)
{
	uint8_t * out;
	ring_buffer buf = NULL;

	buf = rb_create(&props);

	out = rb_pop_tail(buf);

	ck_assert(!out);
	ck_assert(rb_empty(buf));

	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_pop_tail_single)
{
	uint8_t in = 1;
	uint8_t * out;
	ring_buffer buf = NULL;

	/* Create list: [1] */
	buf = rb_create(&props);
	rb_push_head(buf, &in);

	out = rb_pop_tail(buf);

	ck_assert(out);
	ck_assert_int_eq(*out, in);
	ck_assert(rb_empty(buf));

	free(out);

	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_pop_tail_multiple)
{
	uint8_t in[] = {1, 2, 3};
	uint8_t * out[3];
	ring_buffer buf = NULL;

	/* Create list: [3, 2, 1] */
	buf = rb_create(&props);
	rb_push_head(buf, &in[0]);
	rb_push_head(buf, &in[1]);
	rb_push_head(buf, &in[2]);

	out[0] = rb_pop_tail(buf);
	out[1] = rb_pop_tail(buf);
	out[2] = rb_pop_tail(buf);

	ck_assert(out[0]);
	ck_assert(out[1]);
	ck_assert(out[2]);

	ck_assert_int_eq(*out[0], in[0]);
	ck_assert_int_eq(*out[1], in[1]);
	ck_assert_int_eq(*out[2], in[2]);

	free(out[0]);
	free(out[1]);
	free(out[2]);
	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_insert_single)
{
	bool success;
	uint8_t in[] = {1, 2};
	uint8_t * out[2];
	ring_buffer buf = NULL;

	/* Create list: [1] */
	buf = rb_create(&props);
	rb_push_head(buf, &in[0]);

	success = rb_insert(buf, &in[1], 0);

	ck_assert(success);

	out[0] = rb_pop_head(buf);
	out[1] = rb_pop_head(buf);

	ck_assert(out[0]);
	ck_assert(out[1]);
	ck_assert_int_eq(*out[0], in[1]);
	ck_assert_int_eq(*out[1], in[0]);
	ck_assert(rb_empty(buf));

	free(out[0]);
	free(out[1]);
	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_insert_multiple)
{
	uint8_t in[] = {1, 2, 3, 4};
	bool success[3];
	uint8_t * out[4];
	ring_buffer buf = NULL;

	buf = rb_create(&props);

	/* [4, 2, 3, 1] */
	success[0] = rb_insert(buf, &in[0], 0);
	success[1] = rb_insert(buf, &in[1], 0);
	success[2] = rb_insert(buf, &in[2], 1);
	success[3] = rb_insert(buf, &in[3], 3);

	ck_assert(success[0]);
	ck_assert(success[1]);
	ck_assert(success[2]);
	ck_assert(success[3]);

	out[0] = rb_pop_tail(buf);
	out[1] = rb_pop_tail(buf);
	out[2] = rb_pop_tail(buf);
	out[3] = rb_pop_tail(buf);

	ck_assert(out[0]);
	ck_assert(out[1]);
	ck_assert(out[2]);
	ck_assert(out[3]);

	ck_assert_int_eq(*out[0], in[0]);
	ck_assert_int_eq(*out[1], in[2]);
	ck_assert_int_eq(*out[2], in[1]);
	ck_assert_int_eq(*out[3], in[3]);

	free(out[0]);
	free(out[1]);
	free(out[2]);
	free(out[3]);
	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_fetch_empty)
{
	uint8_t * out[2];
	ring_buffer buf;

	buf = rb_create(&props);

	out[0] = rb_fetch(buf, 0);
	out[1] = rb_fetch(buf, 1);

	ck_assert(!out[0]);
	ck_assert(!out[1]);
	ck_assert(rb_empty(buf));

	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_fetch_single)
{
	uint8_t in = 1;
	uint8_t * out;
	ring_buffer buf = NULL;

	/* Create list: [1] */
	buf = rb_create(&props);
	rb_push_head(buf, &in);

	out = rb_fetch(buf, 0);

	ck_assert(out);
	ck_assert_int_eq(*out, in);

	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_fetch_multiple)
{
	uint8_t in[] = {0, 1, 2};
	uint8_t * out[3];
	ring_buffer buf = NULL;

	/* Create list: [1, 2, 3] */
	buf = rb_create(&props);
	rb_push_tail(buf, &in[0]);
	rb_push_tail(buf, &in[1]);
	rb_push_tail(buf, &in[2]);

	out[0] = rb_fetch(buf, 1);
	out[1] = rb_fetch(buf, 0);
	out[2] = rb_fetch(buf, 2);

	ck_assert(out[0]);
	ck_assert(out[1]);
	ck_assert(out[2]);

	ck_assert_int_eq(*out[0], in[1]);
	ck_assert_int_eq(*out[1], in[0]);
	ck_assert_int_eq(*out[2], in[2]);

	rb_destroy(&buf);
}
END_TEST

uint8_t increment(uint8_t n)
{
	return n + 1;
}
WRAP_MAPPABLE(increment, uint8_t, transform);

START_TEST(test_rb_map_empty)
{
	/* `buffer` is initially empty. */
	rb_map(buffer, transform);

	ck_assert(rb_empty(buffer));
}
END_TEST

START_TEST(test_rb_map)
{
	uint8_t in[] = {1, 2, 3};
	uint8_t * out;

	for(size_t i = 0; i < array_size(in); i++)
		rb_push_head(buffer, &in[i]);

	rb_map(buffer, transform);

	ck_assert_int_eq(rb_size(buffer), array_size(in));

	for(size_t i = 0; i < array_size(in); i++) {
		out = rb_pop_tail(buffer);

		ck_assert(out);
		ck_assert_int_eq(*out, in[i] + 1);
	}
}
END_TEST

int8_t subtract(const int8_t a, const int8_t b)
{
	return a - b;
}
WRAP_RFOLDABLE(subtract, int8_t, reduce_right);
WRAP_LFOLDABLE(subtract, int8_t, reduce_left);

START_TEST(test_rb_foldr)
{
	int8_t in[] = {1, 2, 3};
	int8_t init = 0;
	int8_t * out;

	for(size_t i = 0; i < array_size(in); i++)
		rb_push_head(buffer, &in[i]);

	out = rb_foldr(buffer, reduce_right, &init);

	ck_assert(out);
	ck_assert_int_eq(*out, 2);
	ck_assert_int_eq(rb_size(buffer), array_size(in));
}
END_TEST

START_TEST(test_rb_foldl)
{
	int8_t in[] = {1, 2, 3};
	int8_t init = 0;
	int8_t * out;

	for(size_t i = 0; i < array_size(in); i++)
		rb_push_head(buffer, &in[i]);

	out = rb_foldl(buffer, reduce_left, &init);

	ck_assert(out);
	ck_assert_int_eq(*out, -6);
	ck_assert_int_eq(rb_size(buffer), array_size(in));
}
END_TEST

bool gte0(int8_t data)
{
	return data >= 0;
}
WRAP_PREDICATE(gte0, int8_t, pred_gte0);

bool lt0(int8_t data)
{
	return data < 0;
}
WRAP_PREDICATE(lt0, int8_t, pred_lt0);

START_TEST(test_rb_any)
{
	bool any[2];
	int8_t in[] = {1, 2, 3};

	for(size_t i = 0; i < array_size(in); i++)
		rb_push_head(buffer, &in[i]);

	any[0] = rb_any(buffer, pred_gte0);
	any[1] = rb_any(buffer, pred_lt0);

	ck_assert(any[0]);
	ck_assert(!any[1]);
	ck_assert_int_eq(rb_size(buffer), array_size(in));
}
END_TEST

Suite * rb_suite(void)
{
	Suite * suite;
	TCase * case_rb_create;
	TCase * case_rb_push_head;
	TCase * case_rb_push_tail;
	TCase * case_rb_pop_head;
	TCase * case_rb_pop_tail;
	TCase * case_rb_insert;
	TCase * case_rb_fetch;
	TCase * case_rb_map;
	TCase * case_rb_foldr;
	TCase * case_rb_foldl;
	TCase * case_rb_any;
	TCase * case_rb_all;

	suite = suite_create("Ring Buffer");

	case_rb_create    = tcase_create("rb_create");
	case_rb_push_head = tcase_create("rb_push_head");
	case_rb_push_tail = tcase_create("rb_push_tail");
	case_rb_pop_head  = tcase_create("rb_pop_head");
	case_rb_pop_tail  = tcase_create("rb_pop_tail");
	case_rb_insert    = tcase_create("rb_insert");
	case_rb_fetch     = tcase_create("rb_fetch");
	case_rb_map       = tcase_create("rb_map");
	case_rb_foldr     = tcase_create("rb_foldr");
	case_rb_foldl     = tcase_create("rb_foldl");
	case_rb_any       = tcase_create("rb_any");
	case_rb_all       = tcase_create("rb_all");

	tcase_add_checked_fixture(case_rb_create,    setup, takedown);
	tcase_add_checked_fixture(case_rb_push_head, setup, takedown);
	tcase_add_checked_fixture(case_rb_push_tail, setup, takedown);
	tcase_add_checked_fixture(case_rb_map,       setup, takedown);
	tcase_add_checked_fixture(case_rb_foldr,     setup, takedown);
	tcase_add_checked_fixture(case_rb_foldl,     setup, takedown);
	tcase_add_checked_fixture(case_rb_any,       setup, takedown);
	tcase_add_checked_fixture(case_rb_all,       setup, takedown);

	tcase_add_test(case_rb_create,    test_rb_create);
	tcase_add_test(case_rb_push_head, test_rb_push_head_single);
	tcase_add_test(case_rb_push_head, test_rb_push_head_multiple);
	tcase_add_test(case_rb_push_tail, test_rb_push_tail_single);
	tcase_add_test(case_rb_push_tail, test_rb_push_tail_multiple);
	tcase_add_test(case_rb_pop_head,  test_rb_pop_head_empty);
	tcase_add_test(case_rb_pop_head,  test_rb_pop_head_single);
	tcase_add_test(case_rb_pop_head,  test_rb_pop_head_multiple);
	tcase_add_test(case_rb_pop_tail,  test_rb_pop_tail_empty);
	tcase_add_test(case_rb_pop_tail,  test_rb_pop_tail_single);
	tcase_add_test(case_rb_pop_tail,  test_rb_pop_tail_multiple);
	tcase_add_test(case_rb_insert,    test_rb_insert_single);
	tcase_add_test(case_rb_insert,    test_rb_insert_multiple);
	tcase_add_test(case_rb_fetch,     test_rb_fetch_empty);
	tcase_add_test(case_rb_fetch,     test_rb_fetch_single);
	tcase_add_test(case_rb_fetch,     test_rb_fetch_multiple);
	tcase_add_test(case_rb_map,       test_rb_map_empty);
	tcase_add_test(case_rb_map,       test_rb_map);
	tcase_add_test(case_rb_foldr,     test_rb_foldr);
	tcase_add_test(case_rb_foldl,     test_rb_foldl);
	tcase_add_test(case_rb_any,       test_rb_any);

	suite_add_tcase(suite, case_rb_create);
	suite_add_tcase(suite, case_rb_push_head);
	suite_add_tcase(suite, case_rb_push_tail);
	suite_add_tcase(suite, case_rb_pop_head);
	suite_add_tcase(suite, case_rb_pop_tail);
	suite_add_tcase(suite, case_rb_insert);
	suite_add_tcase(suite, case_rb_fetch);
	suite_add_tcase(suite, case_rb_map);
	suite_add_tcase(suite, case_rb_foldr);
	suite_add_tcase(suite, case_rb_foldl);

	return suite;
}

int main(void)
{
	Suite * suite_rb;
	SRunner * suite_runner;

	suite_rb = rb_suite();

	suite_runner = srunner_create(suite_rb);
	srunner_run_all(suite_runner, CK_NORMAL);
	srunner_free(suite_runner);

	return 0;
}
