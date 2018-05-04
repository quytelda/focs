/* revabs_dll.h - Reverse and take the absolute value of a list of numbers
 *                using a doubly linked list as a stack.
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

#include <stdio.h>
#include <stdlib.h>
#include <list/double_list.h>

static int * my_abs(int * n)
{
	*n = abs(*n);
	return n;
}

int main(int argc, char * argv[])
{
	int err;
	int * inputs;
	int ** outputs;

	struct double_list * my_list;

	inputs  = calloc(argc, sizeof(int));
	outputs = calloc(argc, sizeof(int *));
	if(!inputs || !outputs)
		return -1; // out of memory

	err = dl_alloc(&my_list, sizeof(int));
	if(err < 0)
		return -2; // failed to create my_list

	/* Push the numbers onto the stack. */
	for(int i = 1; i < argc; i++) {
		inputs[i] = atoi(argv[i]);
		dl_push_head(my_list, &inputs[i]);
	}

	/* Transform the list by mapping my_abs over the list. */
	dl_map(my_list, (map_fn) my_abs);

	/* Output each number popped from the stack. */
	for(int i = 1; i < argc; i++) {
		outputs[i] = dl_pop_head(my_list);
		printf("%d ", *outputs[i]);
	}

	putchar('\n');

	return 0;
}
