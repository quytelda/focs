/* hof_generics.h - Generic Higher Order Function Macros
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

#include "../../hof.h"

#ifndef __GENERICS_HOF_H
#define __GENERICS_HOF_H

/* ################### *
 * # Transformations # *
 * ################### */
typedef void   (* map_hof_fn)  (void * ds, map_fn fn);
typedef void * (* foldr_hof_fn)(void * ds, foldr_fn fn, const void * init);
typedef void * (* foldl_hof_fn)(void * ds, foldl_fn fn, const void * init);

/* ############## *
 * # Properties # *
 * ############## */
typedef bool (* any_hof_fn)  (void * ds, pred_fn p);
typedef bool (* all_hof_fn)  (void * ds, pred_fn p);

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
	any_hof_fn   any;
	all_hof_fn   all;

	/* Filtering */
	filter_hof_fn     filter;
	drop_while_hof_fn drop_while;
	take_while_hof_fn take_while;
};

#define map(ds, fn)          (__DS_HOF_OPS(ds)->map(ds, fn))
#define foldr(ds, fn, init)  (__DS_HOF_OPS(ds)->foldr(ds, fn, init))
#define foldl(ds, fn, init)  (__DS_HOF_OPS(ds)->foldl(ds, fn, init))
#define any(ds, pred)        (__DS_HOF_OPS(ds)->any(ds, pred))
#define all(ds, pred)        (__DS_HOF_OPS(ds)->all(ds, pred))
#define filter(ds, pred)     (__DS_HOF_OPS(ds)->filter(ds, pred))
#define drop_while(ds, pred) (__DS_HOF_OPS(ds)->drop_while(ds, pred))
#define take_while(ds, pred) (__DS_HOF_OPS(ds)->take_while(ds, pred))

#endif /* __GENERICS_HOF_H */
