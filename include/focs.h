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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef __FOCS_H
#define __FOCS_H

/**
 * mod() - Modulo operation
 * @a The divisor
 * @n The dividend
 *
 * Returns the remainder of the quotient `(@a / @n)`.  This operation is
 * guaranteed to always return a positive integer.  Both @a and @n should be
 * signed types if either @a or @n may be negative.
 *
 * This modulo operation is distinct from the ISO C99 remainder operation
 * represented by the operator '%', which will produce negative results
 * if the divisor is negative and always takes the sign of the dividend.
 *
 * See: https://en.wikipedia.org/wiki/Modulo_operation
 */
#define mod(a, n) (((a) % (n) + (n)) % (n))

typedef void * (* map_fn_t)(void * data);
typedef void * (* foldr_fn_t)(const void * c, void * acc);
typedef void * (* foldl_fn_t)(void * acc, const void * c);
typedef bool   (* comp_fn_t)(const void * a, const void * b);
typedef bool   (* pred_fn_t)(const void * data);

#endif /* __FOCS_H */
