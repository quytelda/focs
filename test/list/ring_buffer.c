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

#include "list/ring_buffer.h"

static const struct ds_properties props = {
	.data_size = sizeof(uint8_t), /* Data size is 1B. */
	.entries   = 10,              /* 10 Data Blocks. */
};

START_TEST(test_rb_create)
{
	ring_buffer buf;

	buf = rb_create(&props);

	ck_assert(buf);

	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_push_head_single)
{
	bool success;
	uint8_t in = 1;
	uint8_t * out;
	ring_buffer buf;

	buf = rb_create(&props);

	success = rb_push_head(buf, &in);
	out = rb_pop_head(buf);

	ck_assert(success);
	ck_assert(out);
	ck_assert_int_eq(*out, in);

	free(out);
	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_push_head_multiple)
{
	bool success[3];
	uint8_t in[] = {1, 2, 3};
	uint8_t * out[3];
	ring_buffer buf = NULL;

	buf = rb_create(&props);

	/* Create list: [3, 2, 1] */
	success[0] = rb_push_head(buf, &in[0]);
	success[1] = rb_push_head(buf, &in[1]);
	success[2] = rb_push_head(buf, &in[2]);

	ck_assert(success[0]);
	ck_assert(success[1]);
	ck_assert(success[2]);

	out[0] = rb_pop_head(buf);
	out[1] = rb_pop_head(buf);
	out[2] = rb_pop_head(buf);

	ck_assert_int_eq(*out[0], in[2]);
	ck_assert_int_eq(*out[1], in[1]);
	ck_assert_int_eq(*out[2], in[0]);

	free(out[0]);
	free(out[1]);
	free(out[2]);
	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_push_tail_single)
{
	bool success;
	uint8_t in = 1;
	uint8_t * out;
	ring_buffer buf;

	buf = rb_create(&props);

	success = rb_push_tail(buf, &in);
	out = rb_pop_tail(buf);

	ck_assert(success);
	ck_assert(out);
	ck_assert_int_eq(*out, in);

	free(out);
	rb_destroy(&buf);
}
END_TEST

START_TEST(test_rb_push_tail_multiple)
{
	bool success[3];
	uint8_t in[] = {1, 2, 3};
	uint8_t * out[3];
	ring_buffer buf = NULL;

	buf = rb_create(&props);

	/* Create list: [3, 2, 1] */
	success[0] = rb_push_tail(buf, &in[0]);
	success[1] = rb_push_tail(buf, &in[1]);
	success[2] = rb_push_tail(buf, &in[2]);

	ck_assert(success[0]);
	ck_assert(success[1]);
	ck_assert(success[2]);

	out[0] = rb_pop_tail(buf);
	out[1] = rb_pop_tail(buf);
	out[2] = rb_pop_tail(buf);

	ck_assert_int_eq(*out[0], in[2]);
	ck_assert_int_eq(*out[1], in[1]);
	ck_assert_int_eq(*out[2], in[0]);

	free(out[0]);
	free(out[1]);
	free(out[2]);
	rb_destroy(&buf);
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

	suite = suite_create("Ring Buffer");

	case_rb_create = tcase_create("rb_create");
	case_rb_push_head = tcase_create("rb_push_head");
	case_rb_push_head = tcase_create("rb_push_tail");
	case_rb_pop_head = tcase_create("rb_pop_head");
	case_rb_pop_tail = tcase_create("rb_pop_tail");
	case_rb_insert = tcase_create("rb_insert");
	case_rb_fetch = tcase_create("rb_fetch");

	tcase_add_test(case_rb_create, test_rb_create);
	tcase_add_test(case_rb_push_head, test_rb_push_head_single);
	tcase_add_test(case_rb_push_head, test_rb_push_head_multiple);
	tcase_add_test(case_rb_push_tail, test_rb_push_tail_single);
	tcase_add_test(case_rb_push_tail, test_rb_push_tail_multiple);
	tcase_add_test(case_rb_pop_head, test_rb_pop_head_empty);
	tcase_add_test(case_rb_pop_head, test_rb_pop_head_single);
	tcase_add_test(case_rb_pop_head, test_rb_pop_head_multiple);
	tcase_add_test(case_rb_pop_tail, test_rb_pop_tail_empty);
	tcase_add_test(case_rb_pop_tail, test_rb_pop_tail_single);
	tcase_add_test(case_rb_pop_tail, test_rb_pop_tail_multiple);
	tcase_add_test(case_rb_insert, test_rb_insert_single);
	tcase_add_test(case_rb_insert, test_rb_insert_multiple);
	tcase_add_test(case_rb_fetch, test_rb_fetch_empty);
	tcase_add_test(case_rb_fetch, test_rb_fetch_single);
	tcase_add_test(case_rb_fetch, test_rb_fetch_multiple);

	suite_add_tcase(suite, case_rb_create);
	suite_add_tcase(suite, case_rb_push_head);
	suite_add_tcase(suite, case_rb_push_tail);
	suite_add_tcase(suite, case_rb_pop_head);
	suite_add_tcase(suite, case_rb_pop_tail);
	suite_add_tcase(suite, case_rb_insert);
	suite_add_tcase(suite, case_rb_fetch);

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
