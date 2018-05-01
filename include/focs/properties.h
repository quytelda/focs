/* focs.h - Functional Open C Structures
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

#ifndef __PROPERTIES_H
#define __PROPERTIES_H

struct data_properties {
	size_t data_size;
	ssize_t entries;
};

#define MAX_DATA_SIZE (~(size_t)  0)
#define MAX_ENTRIES   (~(ssize_t) 0)

#define __DS_PROPS_NAME props
#define __ds_props(ds) ((ds)->__DS_PROPS_NAME)

#define DS_SET_PROPS(ds, _props) (__ds_props(ds) = _props)

/* Access individual data properties. */
#define DS_DATA_SIZE(ds) (__ds_props(ds)->data_size)
#define DS_ENTRIES(ds)   (__ds_props(ds)->entries)

#endif /* __PROPERTIES_H */
