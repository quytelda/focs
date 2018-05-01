/* functional.h - Higher Order Functions Emulation
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

#ifndef __FUNCTIONAL_H
#define __FUNCTIONAL_H

/* Generic function types for use in higher order functions. */
typedef void * (* map_fn)  (void * data);
typedef void * (* foldr_fn)(const void * c, void * acc);
typedef void * (* foldl_fn)(void * acc, const void * c);
typedef bool   (* comp_fn) (const void * a, const void * b);
typedef bool   (* pred_fn) (const void * data);

/* Higher order functions (transformations). */
typedef void   (* map_dop)  (void * ds, map_fn fn);
typedef void * (* foldr_dop)(void * ds, foldr_fn fn,
			     const void * init);
typedef void * (* foldl_dop)(void * ds, foldl_fn fn,
			     const void * init);

/* Higher order functions (properties). */
typedef bool   (* null_dop)    (void * ds);
typedef bool   (* contains_dop)(void * ds, void * data);
typedef bool   (* any_dop)     (void * ds, pred_fn p);
typedef bool   (* all_dop)     (void * ds, pred_fn p);

/* Higher order functions (filtering). */
typedef bool   (* filter_dop)    (void * ds, pred_fn p);
typedef bool   (* drop_while_dop)(void * ds, pred_fn p);
typedef bool   (* take_while_dop)(void * ds, pred_fn p);

struct data_operations {
	/* Transformations */
	map_dop map;
	foldr_dop foldr;
	foldl_dop foldl;
		
	/* Properties */
	null_dop null;
	contains_dop contains;
	any_dop any;
	all_dop all;

	/* Filtering */
	filter_dop filter;
	drop_while_dop drop_while;
	take_while_dop take_while;
};

#define __DS_DOPS_NAME dops
#define __ds_dops(ds) ((ds)->__DS_DOPS_NAME)

#define map(ds, fn)          (__ds_dops(ds)->map(ds, fn))
#define foldr(ds, fn, init)  (__ds_dops(ds)->foldr(ds, fn, init))
#define foldl(ds, fn, init)  (__ds_dops(ds)->foldl(ds, fn, init))
#define null(ds)             (__ds_dops(ds)->null(ds))
#define contains(ds, datum)  (__ds_dops(ds)->contains(ds, datum))
#define any(ds, pred)        (__ds_dops(ds)->any(ds, pred))
#define all(ds, pred)        (__ds_dops(ds)->all(ds, pred))
#define filter(ds, pred)     (__ds_dops(ds)->filter(ds, pred))
#define drop_while(ds, pred) (__ds_dops(ds)->drop_while(ds, pred))
#define take_while(ds, pred) (__ds_dops(ds)->take_while(ds, pred))

#endif /* __FUNCTIONAL_H */
