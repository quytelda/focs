/* mgmt_operations.h - Data Structure Management Operations
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

#ifndef __MGMT_OPERATIONS_H
#define __MGMT_OPERATIONS_H

#include "focs/data_structure.h"

typedef void * (* create_mgmt_fn) (const struct ds_properties * props);
typedef void   (* destroy_mgmt_fn)(void * ds);

struct mgmt_operations {
	create_mgmt_fn  create;
	destroy_mgmt_fn destroy;
};

#endif /* __MGMT_OPERATIONS_H */
