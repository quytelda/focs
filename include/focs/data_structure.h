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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef GENERICS
#include "focs/hof_operations.h"
#include "focs/mgmt_operations.h"
#endif

#ifndef __DATA_STRUCTURE_H
#define __DATA_STRUCTURE_H

struct ds_properties {
	size_t data_size;
	size_t entries;
	bool   overwrite;
};

#define __DS_HOF_OPS_NAME   __hof_ops
#define __DS_MGMT_OPS_NAME  __mgmt_ops
#define __DS_PRIV_NAME      __priv
#define __DS_PROPS_NAME     __props

#ifdef GENERICS
#define START_DS(ds_name)						\
	typedef struct {						\
		const struct ds_properties   * __DS_PROPS_NAME;		\
		const struct hof_operations  * __DS_HOF_OPS_NAME;	\
		const struct mgmt_operations * __DS_MGMT_OPS_NAME;	\
		struct ds_name##_priv

#define __DS_HOF_OPS(ds)  ((ds)->__DS_HOF_OPS_NAME)
#define __DS_MGMT_OPS(ds) ((ds)->__DS_MGMT_OPS_NAME)

#define DS_INIT(ds, props, mgmt_ops, hof_ops)	\
	({					\
		DS_PROPS(ds)      = props;	\
		__DS_MGMT_OPS(ds) = mgmt_ops;	\
		__DS_HOF_OPS(ds)  = hof_ops;	\
	})

#else /* GENERICS */

#define START_DS(ds_name)				\
	typedef struct {				\
	const struct ds_properties * __DS_PROPS_NAME;	\
	struct ds_name##_priv

#define DS_INIT(ds, props, mgmt_ops, hof_ops) (DS_PROPS(ds) = props)

#endif /* GENERICS */

#define END_DS(ds_name) __DS_PRIV_NAME; } * ds_name

#define DS_PROPS(ds) ((ds)->__DS_PROPS_NAME)
#define DS_PRIV(ds) (&((ds)->__DS_PRIV_NAME))

#define DS_DATA_SIZE(ds) (DS_PROPS(ds)->data_size)
#define DS_ENTRIES(ds)   (DS_PROPS(ds)->entries)
#define DS_OVERWRITE(ds) (DS_PROPS(ds)->overwrite)

#define DS_ALLOC(ds) (ds = malloc(sizeof(*ds)))
#define DS_FREE(ds) (free_null(*ds))

#endif /* __DATA_STRUCTURE_H */
