/* hof.h - Higher Order Functions Emulation
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

#ifndef __HOF_H
#define __HOF_H

/* Generic function types for use in higher order functions. */
typedef void * (* map_fn)  (void * data);
typedef void * (* foldr_fn)(const void * c, void * acc);
typedef void * (* foldl_fn)(void * acc, const void * c);
typedef bool   (* comp_fn) (const void * a, const void * b);
typedef bool   (* pred_fn) (const void * data);

#endif /* __HOF_H */
