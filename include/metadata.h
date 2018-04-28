/* metadata.h - Storage Container Metadata & Properties.
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

#include "focs.h"

#ifndef __METADATA_H
#define __METADATA_H

struct data_properties {
	size_t data_size;
	size_t entries;
};

typedef void   (* map_dop_t)(void * ds, map_fn_t fn);
typedef void * (* foldr_dop_t)(void * ds, foldr_fn_t fn, const void * init);
typedef void * (* foldl_dop_t)(void * ds, foldl_fn_t fn, const void * init);
typedef bool   (* null_dop_t)(void * ds);
typedef bool   (* contains_dop_t)(void * ds, void * data);
typedef bool   (* any_dop_t)(void * ds, pred_fn_t p);
typedef bool   (* all_dop_t)(void * ds, pred_fn_t p);
typedef bool   (* filter_dop_t)(void * ds, pred_fn_t p);
typedef bool   (* drop_while_dop_t)(void * ds, pred_fn_t p);
typedef bool   (* take_while_dop_t)(void * ds, pred_fn_t p);

struct data_operations {
	/* Transformations */
	map_dop_t map;
	foldr_dop_t foldr;
	foldl_dop_t foldl;
		
	/* Properties */
	null_dop_t null;
	contains_dop_t contains;
	any_dop_t any;
	all_dop_t all;

	/* Filtering */
	filter_dop_t filter;
	drop_while_dop_t drop_while;
	take_while_dop_t take_while;
};

#define DS_METADATA				\
	const struct data_properties * props;	\
	const struct data_operations * dops;

#define __ds_dops(ds)  ((ds)->dops)
#define __ds_props(ds) ((ds)->props)

#define DS_METADATA_INIT(ds, _props, _dops)	\
	do {					\
		__ds_dops(ds)  = _dops;		\
		__ds_props(ds) = _props;	\
	} while(0)

#define DS_DATA_SIZE(ds) (__ds_props(ds)->data_size)
#define DS_ENTRIES(ds)   (__ds_props(ds)->entries)

#define map(ds, fn)          __ds_dops(ds)->map(ds, fn)
#define foldr(ds, fn, init)  __ds_dops(ds)->foldr(ds, fn, init)
#define foldl(ds, fn, init)  __ds_dops(ds)->foldl(ds, fn, init)
#define null(ds)             __ds_dops(ds)->null(ds)
#define contains(ds, datum)  __ds_dops(ds)->contains(ds, datum)
#define any(ds, pred)        __ds_dops(ds)->any(ds, pred)
#define all(ds, pred)        __ds_dops(ds)->all(ds, pred)
#define filter(ds, pred)     __ds_dops(ds)->filter(ds, pred)
#define drop_while(ds, pred) __ds_dops(ds)->drop_while(ds, pred)
#define take_while(ds, pred) __ds_dops(ds)->take_while(ds, pred)

#endif /* __METADATA_H */
