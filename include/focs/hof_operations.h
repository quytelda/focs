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

struct hof_operations {
	/* Transformations */
	map_hof   map;
	foldr_hof foldr;
	foldl_hof foldl;
		
	/* Properties */
	empty_hof empty;
	elem_hof  contains;
	any_hof   any;
	all_hof   all;

	/* Filtering */
	filter_hof     filter;
	drop_while_hof drop_while;
	take_while_hof take_while;
};

#define map(ds, fn)          (__DS_DOPS(ds)->map(ds, fn))
#define foldr(ds, fn, init)  (__DS_DOPS(ds)->foldr(ds, fn, init))
#define foldl(ds, fn, init)  (__DS_DOPS(ds)->foldl(ds, fn, init))
#define null(ds)             (__DS_DOPS(ds)->null(ds))
#define contains(ds, datum)  (__DS_DOPS(ds)->contains(ds, datum))
#define any(ds, pred)        (__DS_DOPS(ds)->any(ds, pred))
#define all(ds, pred)        (__DS_DOPS(ds)->all(ds, pred))
#define filter(ds, pred)     (__DS_DOPS(ds)->filter(ds, pred))
#define drop_while(ds, pred) (__DS_DOPS(ds)->drop_while(ds, pred))
#define take_while(ds, pred) (__DS_DOPS(ds)->take_while(ds, pred))

#endif /* __HOF_OPERATIONS_H */
