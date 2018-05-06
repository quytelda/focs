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

#ifndef __FOCS_H
#define __FOCS_H

/**
 * Pick the minimum of two comparable values.
 * @param m First comparable value
 * @param n Second comparable value
 *
 * @return the the lesser of two comparable parameters `m` and `n`.
 * In the case that `m` and `n` are compared and found to be equal,
 * then `m` is returned.
 */
#define MIN(m, n)				\
	({					\
		typeof(m) m_ = m;		\
		typeof(n) n_ = n;		\
		((m_ <= n_) ? m_ : n_);		\
	})

/**
 * Pick the maximum of two comparable values.
 * @param m First comparable value
 * @param n Second comparable value
 *
 * @return the the greater of two comparable parameters `m` and `n`.
 * In the case that `m` and `n` are compared and found to be equal,
 * then `m` is returned.
 */
#define MAX(m, n)				\
	({					\
		typeof(m) m_ = m;		\
		typeof(n) n_ = n;		\
		((m_ >= n_) ? m_ : n_);		\
	})

/**
 * Set errno and return from the current function with some value.
 * @param err The error number to set errno to
 * @param val The optional value to return
 *
 * Set errno to `err`, then return from the current function.  The value
 * supplied as the second argument will be the return value of the function.
 */
#define return_with_errno(err, val)		\
	({					\
		errno = err;			\
		return val;			\
	})

/**
 * Set errno then jump to the specified label.
 * @param err The error number to set errno to
 * @param lable The label to jump to
 *
 * Set errno to `err`, then execute `goto label` inside the function,
 * effectively jumping to `label`.
 */
#define goto_with_errno(err, label)		\
	do {					\
		errno = err;			\
		goto label;			\
	} while(0)

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

#endif /* __FOCS_H */
