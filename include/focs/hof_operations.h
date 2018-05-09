/* hof_operations.h - Higher Order Functions Emulation
 * Copyright (C) 2018 Quytelda Kahja
 *
 * This file is part of focs.
 *
 * focs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#include "focs/hof.h"

#ifndef __HOF_OPERATIONS_H
#define __HOF_OPERATIONS_H

/* ################### *
 * # Transformations # *
 * ################### */
typedef void   (* map_hof_fn)  (void * ds, map_fn fn);
typedef void * (* foldr_hof_fn)(void * ds, foldr_fn fn, const void * init);
typedef void * (* foldl_hof_fn)(void * ds, foldl_fn fn, const void * init);

/* ############## *
 * # Properties # *
 * ############## */
typedef bool (* empty_hof_fn)(void * ds);
typedef bool (* elem_hof_fn)(void * ds, void * data);
typedef bool (* any_hof_fn) (void * ds, pred_fn p);
typedef bool (* all_hof_fn) (void * ds, pred_fn p);

/* ############# *
 * # Filtering # *
 * ############# */
typedef bool (* filter_hof_fn)    (void * ds, pred_fn p);
typedef bool (* drop_while_hof_fn)(void * ds, pred_fn p);
typedef bool (* take_while_hof_fn)(void * ds, pred_fn p);

struct hof_operations {
	/* Transformations */
	map_hof_fn   map;
	foldr_hof_fn foldr;
	foldl_hof_fn foldl;

	/* Properties */
	empty_hof_fn empty;
	elem_hof_fn  elem;
	any_hof_fn   any;
	all_hof_fn   all;

	/* Filtering */
	filter_hof_fn     filter;
	drop_while_hof_fn drop_while;
	take_while_hof_fn take_while;
};

#endif /* __HOF_OPERATIONS_H */
