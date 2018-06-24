/* ds.h - Generic Data Structure API
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

#ifndef __FOCS_DS_H
#define __FOCS_DS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct ds_properties {
	size_t data_size;
	size_t entries;
	bool   overwrite;
};

#define __DS_PRIV_NAME  __priv
#define __DS_PROPS_NAME __props

#define DS_START(ds_name)                                          \
	typedef struct {                                           \
		const struct ds_properties   * __DS_PROPS_NAME;    \
		struct ds_name##_priv
#define DS_ALLOC(ds)       (ds = malloc(sizeof(*ds)))
#define DS_INIT(ds, props) (DS_PROPS(ds) = props)
#define DS_FREE(ds)        (free_null(*ds))
#define DS_END(ds_name) __DS_PRIV_NAME; } * ds_name

#define DS_PROPS(ds) ((ds)->__DS_PROPS_NAME)
#define DS_PRIV(ds)  (&((ds)->__DS_PRIV_NAME))

#define DS_DATA_SIZE(ds) (DS_PROPS(ds)->data_size)
#define DS_ENTRIES(ds)   (DS_PROPS(ds)->entries)
#define DS_OVERWRITE(ds) (DS_PROPS(ds)->overwrite)

#define DS_DATA_EQ(ds, s1, s2) (memcmp(s1, s2, DS_DATA_SIZE(ds)) == 0)

#endif /* __FOCS_DS_H */
