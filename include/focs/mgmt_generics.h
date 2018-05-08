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

#include "focs/generics.h"

#ifndef __MGMT_GENERICS_H
#define __MGMT_GENERICS_H

#define create(ds, props) (__DS_MGMT_OPS(ds)->create(ds, props))
#define destroy(ds)       (__DS_MGMT_OPS(ds)->destroy(ds))
#define size(ds)          (__DS_MGMT_OPS(ds)->size(ds))

#endif /* __MGMT_GENERICS_H */
