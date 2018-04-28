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
	.data_size = sizeof(uint8_t),
	.entries   = 100,
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
	ck_assert_int_eq(DS_ENTRIES(buf), 100);

	rb_free(&buf);
}
END_TEST

Suite * rb_suite(void)
{
	Suite * suite;
	TCase * case_rb_alloc;

	suite = suite_create("Ring Buffer");

	case_rb_alloc = tcase_create("rb_alloc");

	tcase_add_test(case_rb_alloc, test_rb_alloc);

	suite_add_tcase(suite, case_rb_alloc);

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
