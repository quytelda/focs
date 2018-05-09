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

typedef size_t (* size_mgmt_fn)   (void * ds);
typedef void   (* destroy_mgmt_fn)(void * ds);

struct mgmt_operations {
	size_mgmt_fn    size;
	destroy_mgmt_fn destroy;
};

#define size(ds)    (__DS_MGMT_OPS(ds)->size(ds))
#define destroy(ds) (__DS_MGMT_OPS(ds)->destroy(ds))

#endif /* __GENERICS_MGMT_H */
