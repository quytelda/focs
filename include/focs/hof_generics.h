/* hof_generics.h - Generic Higher Order Function Macros
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

#include "focs/data_structure.h"

#ifndef __HOF_GENERICS_H
#define __HOF_GENERICS_H

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

#endif /* __HOF_GENERICS_H */
