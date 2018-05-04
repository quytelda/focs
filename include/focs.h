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
#include <unistd.h>

#include "focs/functional.h"
#include "focs/properties.h"

#ifndef __FOCS_H
#define __FOCS_H

#define DECLARE_DS(dops)				\
	const struct data_properties * __DS_PROPS_NAME;	\
	const struct data_operations * __DS_DOPS_NAME

/**
 * Modulo operation
 * @param a The divisor
 * @param n The dividend
 *
 * This modulo operation is distinct from the ISO C99 remainder operation
 * represented by the operator '%', which will produce negative results
 * if the divisor is negative and always takes the sign of the dividend.
 *
 * This operation is guaranteed to always return a positive integer.  Both `a`
 * and `n` should be signed types if either `a` or `n` may be negative.
 *
 * See: https://en.wikipedia.org/wiki/Modulo_operation
 *
 * @return The remainder of the quotient `(a / n)`.

 */
#define mod(a, n) (((a) % (n) + (n)) % (n))

/**
 * Set errno and return from the current function with some value.
 * @param err The error number to set errno to
 * @param val The optional value to return
 *
 * Set errno to `err`, then execute `return val` inside the function,
 * effectively return `val`, or nothing if `val` is left empty.
 */
#define return_with_errno(err, val...) \
	do {			       \
		errno = err;	       \
		return val;	       \
	} while(0)

#endif /* __FOCS_H */
