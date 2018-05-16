/* mgmt_generics.h - Generic Data Structure Management Macros
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

#ifndef __GENERICS_MGMT_H
#define __GENERICS_MGMT_H

typedef bool   (* elem_mgmt_fn)  (void * ds, void * data);
typedef bool   (* empty_mgmt_fn) (void * ds);
typedef bool   (* full_mgmt_fn)  (void * ds);
typedef size_t (* size_mgmt_fn)  (void * ds);

struct mgmt_operations {
	elem_mgmt_fn  elem;
	empty_mgmt_fn empty;
	full_mgmt_fn  full;
	size_mgmt_fn  size;
};

#define elem(ds, datum) (__DS_MGMT_OPS(ds)->elem(ds, datum))
#define empty(ds)       (__DS_MGMT_OPS(ds)->empty(ds))
#define full(ds)        (__DS_MGMT_OPS(ds)->full(ds))
#define size(ds)        (__DS_MGMT_OPS(ds)->size(ds))

#endif /* __GENERICS_MGMT_H */
