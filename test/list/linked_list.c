#include <check.h>
#include <errno.h>
#include <stdio.h>

#include "list/linked_list.h"

START_TEST(test_linklist_alloc)
{
	int err;
	struct linked_list * list = NULL;

	err = linklist_alloc(&list, 0);
	ck_assert(!err);
	ck_assert(list);
	linklist_free(&list);
}
END_TEST

START_TEST(test_linklist_null_true)
{
	struct linked_list * list;

	linklist_alloc(&list, 0);
	ck_assert(linklist_null(list));
	linklist_free(&list);
}
END_TEST

START_TEST(test_linklist_null_false)
{
	uint8_t val = 1;
	struct linked_list * list;

	linklist_alloc(&list, sizeof(val));
	linklist_push_head(list, &val);
	ck_assert(!linklist_null(list));
	linklist_free(&list);
}
END_TEST

START_TEST(test_linklist_push_head_single)
{
	uint8_t val = 1;
	struct linked_list * list;

	linklist_alloc(&list, sizeof(uint8_t));
	linklist_push_head(list, &val);
	ck_assert(list->head);
	ck_assert(list->tail);
	ck_assert_mem_eq(list->head->data, &val, sizeof(uint8_t));
	linklist_free(&list);

}
END_TEST

START_TEST(test_linklist_push_head_multiple)
{
	uint8_t val1 = 1;
	uint8_t val2 = 2;
	uint8_t val3 = 3;
	struct linked_list * list;

	linklist_alloc(&list, sizeof(uint8_t));
	linklist_push_head(list, &val1);
	linklist_push_head(list, &val2);
	linklist_push_head(list, &val3);

	struct element * current = list->head;
	ck_assert_mem_eq(current->data, &val3, sizeof(uint8_t));
	current = current->next;
	ck_assert_mem_eq(current->data, &val2, sizeof(uint8_t));
	current = current->next;
	ck_assert_mem_eq(current->data, &val1, sizeof(uint8_t));
	linklist_free(&list);

}
END_TEST

START_TEST(test_linklist_push_tail_single)
{
	uint8_t val = 1;
	struct linked_list * list;

	linklist_alloc(&list, sizeof(uint8_t));
	linklist_push_tail(list, &val);
	ck_assert(list->head);
	ck_assert(list->tail);
	ck_assert_mem_eq(list->head->data, &val, sizeof(uint8_t));
	linklist_free(&list);

}
END_TEST

START_TEST(test_linklist_push_tail_multiple)
{
	uint8_t val1 = 1;
	uint8_t val2 = 2;
	uint8_t val3 = 3;
	struct linked_list * list;

	linklist_alloc(&list, sizeof(uint8_t));
	linklist_push_tail(list, &val1);
	linklist_push_tail(list, &val2);
	linklist_push_tail(list, &val3);

	struct element * current = list->head;
	ck_assert_mem_eq(current->data, &val1, sizeof(uint8_t));
	current = current->next;
	ck_assert_mem_eq(current->data, &val2, sizeof(uint8_t));
	current = current->next;
	ck_assert_mem_eq(current->data, &val3, sizeof(uint8_t));

	linklist_free(&list);
}
END_TEST

START_TEST(test_linklist_pop_head_single)
{
	uint8_t in = 1;
	struct linked_list * list;

	linklist_alloc(&list, sizeof(uint8_t));
	linklist_push_head(list, &in);

	ck_assert_mem_eq(linklist_pop_head(list), &in, sizeof(in));
	ck_assert(!list->head);
	ck_assert(!list->tail);

	linklist_free(&list);
}
END_TEST

START_TEST(test_linklist_pop_head_multiple)
{
	uint8_t in1 = 1;
	uint8_t in2 = 2;
	uint8_t in3 = 3;
	struct linked_list * list;

	linklist_alloc(&list, sizeof(uint8_t));

	linklist_push_head(list, &in1);
	linklist_push_head(list, &in2);
	linklist_push_head(list, &in3);

	ck_assert_mem_eq(linklist_pop_head(list), &in3, sizeof(uint8_t));
	ck_assert_mem_eq(linklist_pop_head(list), &in2, sizeof(uint8_t));
	ck_assert_mem_eq(linklist_pop_head(list), &in1, sizeof(uint8_t));

	ck_assert(!list->head);
	ck_assert(!list->tail);

	linklist_free(&list);
}
END_TEST

START_TEST(test_linklist_pop_tail_single)
{
	uint8_t in = 1;
	struct linked_list * list;

	linklist_alloc(&list, sizeof(uint8_t));
	linklist_push_tail(list, &in);

	ck_assert_mem_eq(linklist_pop_tail(list), &in, sizeof(in));
	ck_assert(!list->head);
	ck_assert(!list->tail);

	linklist_free(&list);
}
END_TEST

START_TEST(test_linklist_pop_tail_multiple)
{
	uint8_t in1 = 1;
	uint8_t in2 = 2;
	uint8_t in3 = 3;
	struct linked_list * list;

	linklist_alloc(&list, sizeof(uint8_t));

	linklist_push_tail(list, &in1);
	linklist_push_tail(list, &in2);
	linklist_push_tail(list, &in3);

	ck_assert_mem_eq(linklist_pop_tail(list), &in3, sizeof(uint8_t));
	ck_assert_mem_eq(linklist_pop_tail(list), &in2, sizeof(uint8_t));
	ck_assert_mem_eq(linklist_pop_tail(list), &in1, sizeof(uint8_t));

	ck_assert(!list->head);
	ck_assert(!list->tail);

	linklist_free(&list);
}
END_TEST

START_TEST(test_linklist_insert_single)
{
	uint8_t in = 1;
	bool success;
	struct linked_list * list;

	linklist_alloc(&list, sizeof(uint8_t));

	success = linklist_insert(list, &in, 0);

	ck_assert(success);
	ck_assert(list->head);
	ck_assert(list->tail);

	linklist_free(&list);
}
END_TEST

START_TEST(test_linklist_insert_multiple)
{
	bool r1;
	bool r2;
	bool r3;
	bool r4;
	uint8_t in1 = 1;
	uint8_t in2 = 2;
	uint8_t in3 = 3;
	uint8_t in4 = 4;
	struct linked_list * list;

	linklist_alloc(&list, sizeof(uint8_t));

	/* Insert the elements out of order. */
	r1 = linklist_insert(list, &in1, 0);
	r2 = linklist_insert(list, &in3, 1);
	r3 = linklist_insert(list, &in2, 1);
	r4 = linklist_insert(list, &in4, 0);

	ck_assert(r1);
	ck_assert(r2);
	ck_assert(r3);
	ck_assert(r4);

	ck_assert(list->head);
	ck_assert(list->tail);

	ck_assert_mem_eq(linklist_pop_head(list), &in4, sizeof(uint8_t));
	ck_assert_mem_eq(linklist_pop_head(list), &in1, sizeof(uint8_t));
	ck_assert_mem_eq(linklist_pop_head(list), &in2, sizeof(uint8_t));
	ck_assert_mem_eq(linklist_pop_head(list), &in3, sizeof(uint8_t));

	linklist_free(&list);
}
END_TEST

Suite * linklist_suite(void)
{
	Suite * suite;
	TCase * case_linklist_alloc;
	TCase * case_linklist_null;
	TCase * case_linklist_push_head;
	TCase * case_linklist_push_tail;
	TCase * case_linklist_pop_head;
	TCase * case_linklist_pop_tail;
	TCase * case_linklist_insert;

	suite = suite_create("Linked List");

	case_linklist_alloc = tcase_create("linklist_alloc");
	case_linklist_null = tcase_create("linklist_null");
	case_linklist_push_head = tcase_create("linklist_push_head");
	case_linklist_push_tail = tcase_create("linklist_push_tail");
	case_linklist_pop_head = tcase_create("linklist_pop_head");
	case_linklist_pop_tail = tcase_create("linklist_pop_tail");
	case_linklist_insert = tcase_create("linklist_insert");

	tcase_add_test(case_linklist_alloc, test_linklist_alloc);
	tcase_add_test(case_linklist_null, test_linklist_null_true);
	tcase_add_test(case_linklist_null, test_linklist_null_false);
	tcase_add_test(case_linklist_push_head, test_linklist_push_head_single);
	tcase_add_test(case_linklist_push_head, test_linklist_push_head_multiple);
	tcase_add_test(case_linklist_push_tail, test_linklist_push_tail_single);
	tcase_add_test(case_linklist_push_tail, test_linklist_push_tail_multiple);
	tcase_add_test(case_linklist_pop_head, test_linklist_pop_head_single);
	tcase_add_test(case_linklist_pop_tail, test_linklist_pop_tail_multiple);
	tcase_add_test(case_linklist_pop_tail, test_linklist_pop_tail_single);
	tcase_add_test(case_linklist_insert, test_linklist_insert_multiple);
	tcase_add_test(case_linklist_insert, test_linklist_insert_single);

	suite_add_tcase(suite, case_linklist_alloc);
	suite_add_tcase(suite, case_linklist_null);
	suite_add_tcase(suite, case_linklist_push_head);
	suite_add_tcase(suite, case_linklist_push_tail);
	suite_add_tcase(suite, case_linklist_pop_head);
	suite_add_tcase(suite, case_linklist_pop_tail);
	suite_add_tcase(suite, case_linklist_insert);

	return suite;
}

int main(void)
{
	Suite * suite_linklist;
	SRunner * suite_runner;

	suite_linklist = linklist_suite();

	suite_runner = srunner_create(suite_linklist);
	srunner_run_all(suite_runner, CK_NORMAL);
	srunner_free(suite_runner);

	return 0;
}
