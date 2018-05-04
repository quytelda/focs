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

static const struct data_properties props = {
	.data_size = sizeof(uint8_t), /* Data size is 1B. */
	.entries   = 10,
};

START_TEST(test_rb_alloc)
{
	int err;
	struct ring_buffer * buf = NULL;

	err = rb_alloc(&buf, &props);

	ck_assert(!err);
	ck_assert(buf);
	ck_assert_int_eq(buf->length, 0);
	ck_assert_int_eq(DS_DATA_SIZE(buf), sizeof(uint8_t));
	ck_assert_int_eq(DS_ENTRIES(buf), 10);

	rb_free(&buf);
}
END_TEST

START_TEST(test_rb_push_head_single)
{
	bool success;
	uint8_t in = 1;
	uint8_t * out;
	struct ring_buffer * buf = NULL;

	rb_alloc(&buf, &props);

	success = rb_push_head(buf, &in);
	out = (uint8_t *) buf->head;

	ck_assert(success);
	ck_assert(out);
	ck_assert_int_eq(*out, in);
	ck_assert_int_eq(buf->length, 1);

	rb_free(&buf);
}
END_TEST

START_TEST(test_rb_push_head_multiple)
{
	bool success[3];
	uint8_t in[] = {1, 2, 3};
	uint8_t * out[3];
	struct ring_buffer * buf = NULL;

	rb_alloc(&buf, &props);

	/* Create list: [3, 2, 1] */
	success[0] = rb_push_head(buf, &in[0]);
	success[1] = rb_push_head(buf, &in[1]);
	success[2] = rb_push_head(buf, &in[2]);

	ck_assert(success[0]);
	ck_assert(success[1]);
	ck_assert(success[2]);
	ck_assert_int_eq(buf->length, 3);

	out[0] = (uint8_t *) buf->head + (0 * sizeof(uint8_t));
	out[1] = (uint8_t *) buf->head + (1 * sizeof(uint8_t));
	out[2] = (uint8_t *) buf->head + (2 * sizeof(uint8_t));

	ck_assert_int_eq(*out[0], in[2]);
	ck_assert_int_eq(*out[1], in[1]);
	ck_assert_int_eq(*out[2], in[0]);

	rb_free(&buf);
}
END_TEST

START_TEST(test_rb_push_tail_single)
{
	bool success;
	uint8_t in = 1;
	uint8_t * out;
	struct ring_buffer * buf = NULL;

	rb_alloc(&buf, &props);

	success = rb_push_tail(buf, &in);
	out = (uint8_t *) buf->tail - sizeof(uint8_t);

	ck_assert(success);
	ck_assert(out);
	ck_assert_int_eq(*out, in);
	ck_assert_int_eq(buf->length, 1);

	rb_free(&buf);
}
END_TEST

START_TEST(test_rb_push_tail_multiple)
{
	bool success[3];
	uint8_t in[] = {1, 2, 3};
	uint8_t * out[3];
	struct ring_buffer * buf = NULL;

	rb_alloc(&buf, &props);

	/* Create list: [3, 2, 1] */
	success[0] = rb_push_tail(buf, &in[0]);
	success[1] = rb_push_tail(buf, &in[1]);
	success[2] = rb_push_tail(buf, &in[2]);

	ck_assert(success[0]);
	ck_assert(success[1]);
	ck_assert(success[2]);
	ck_assert_int_eq(buf->length, 3);

	out[0] = (uint8_t *) buf->tail - (1 * sizeof(uint8_t));
	out[1] = (uint8_t *) buf->tail - (2 * sizeof(uint8_t));
	out[2] = (uint8_t *) buf->tail - (3 * sizeof(uint8_t));

	ck_assert_int_eq(*out[0], in[2]);
	ck_assert_int_eq(*out[1], in[1]);
	ck_assert_int_eq(*out[2], in[0]);

	rb_free(&buf);
}
END_TEST

START_TEST(test_rb_pop_head_empty)
{
	uint8_t * out;
	struct ring_buffer * buf = NULL;

	rb_alloc(&buf, &props);

	out = rb_pop_head(buf);

	ck_assert(!out);
	ck_assert_ptr_eq(buf->head, buf->data);
	ck_assert_ptr_eq(buf->tail, buf->data);
	ck_assert(buf->length == 0);

	rb_free(&buf);
}
END_TEST

START_TEST(test_rb_pop_head_single)
{
	uint8_t in = 1;
	uint8_t * out;
	struct ring_buffer * buf = NULL;

	/* Create list: [1] */
	rb_alloc(&buf, &props);
	rb_push_head(buf, &in);

	out = rb_pop_head(buf);

	ck_assert(out);
	ck_assert_int_eq(*out, in);
	ck_assert(buf->length == 0);

	free(out);
	rb_free(&buf);
}
END_TEST

START_TEST(test_rb_pop_head_multiple)
{
	uint8_t in[] = {1, 2, 3};
	uint8_t * out[3];
	struct ring_buffer * buf = NULL;

	/* Create list: [3, 2, 1] */
	rb_alloc(&buf, &props);
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
	rb_free(&buf);
}
END_TEST

START_TEST(test_rb_pop_tail_empty)
{
	uint8_t * out;
	struct ring_buffer * buf = NULL;

	rb_alloc(&buf, &props);

	out = rb_pop_tail(buf);

	ck_assert(!out);
	ck_assert_ptr_eq(buf->head, buf->data);
	ck_assert_ptr_eq(buf->tail, buf->data);
	ck_assert(buf->length == 0);

	rb_free(&buf);
}
END_TEST

START_TEST(test_rb_pop_tail_single)
{
	uint8_t in = 1;
	uint8_t * out;
	struct ring_buffer * buf = NULL;

	/* Create list: [1] */
	rb_alloc(&buf, &props);
	rb_push_head(buf, &in);

	out = rb_pop_tail(buf);

	ck_assert(out);
	ck_assert_int_eq(*out, in);
	ck_assert(buf->length == 0);

	free(out);

	rb_free(&buf);
}
END_TEST

START_TEST(test_rb_pop_tail_multiple)
{
	uint8_t in[] = {1, 2, 3};
	uint8_t * out[3];
	struct ring_buffer * buf = NULL;

	/* Create list: [3, 2, 1] */
	rb_alloc(&buf, &props);
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
	rb_free(&buf);
}
END_TEST

START_TEST(test_rb_insert_single)
{
	bool success;
	uint8_t in = 1;
	uint8_t * out;
	struct ring_buffer * buf = NULL;

	/* Create list: [1] */
	rb_alloc(&buf, &props);

	success = rb_insert(buf, &in, 0);

	ck_assert(success);

	out = rb_pop_tail(buf);

	ck_assert(out);
	ck_assert_int_eq(*out, in);
	ck_assert(buf->length == 0);

	free(out);
	rb_free(&buf);
}
END_TEST

START_TEST(test_rb_insert_multiple)
{
	uint8_t in[] = {1, 2, 3, 4};
	bool success[sizeof(in)];
	uint8_t * out[sizeof(in)];
	struct ring_buffer * buf = NULL;

	rb_alloc(&buf, &props);

	/* [1, 4, 3, 2] */
	success[0] = rb_insert(buf, &in[0], 0);
	success[1] = rb_insert(buf, &in[1], 1);
	success[2] = rb_insert(buf, &in[2], 1);
	success[3] = rb_insert(buf, &in[3], 1);

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

	ck_assert_int_eq(*out[0], in[1]);
	ck_assert_int_eq(*out[1], in[2]);
	ck_assert_int_eq(*out[2], in[3]);
	ck_assert_int_eq(*out[3], in[0]);

	free(out[0]);
	free(out[1]);
	free(out[2]);
	free(out[3]);
	rb_free(&buf);
}
END_TEST

Suite * rb_suite(void)
{
	Suite * suite;
	TCase * case_rb_alloc;
	TCase * case_rb_push_head;
	TCase * case_rb_push_tail;
	TCase * case_rb_pop_head;
	TCase * case_rb_pop_tail;
	TCase * case_rb_insert;

	suite = suite_create("Ring Buffer");

	case_rb_alloc = tcase_create("rb_alloc");
	case_rb_push_head = tcase_create("rb_push_head");
	case_rb_push_head = tcase_create("rb_push_tail");
	case_rb_pop_head = tcase_create("rb_pop_head");
	case_rb_pop_tail = tcase_create("rb_pop_tail");
	case_rb_insert = tcase_create("rb_insert");

	tcase_add_test(case_rb_alloc, test_rb_alloc);
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

	suite_add_tcase(suite, case_rb_alloc);
	suite_add_tcase(suite, case_rb_push_head);
	suite_add_tcase(suite, case_rb_push_tail);
	suite_add_tcase(suite, case_rb_pop_head);
	suite_add_tcase(suite, case_rb_pop_tail);
	suite_add_tcase(suite, case_rb_insert);

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
