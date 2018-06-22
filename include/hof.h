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
typedef void (* map_fn)  (void * data);
typedef bool (* comp_fn) (const void * a, const void * b);
typedef bool (* pred_fn) (const void * data);
typedef void (* foldr_fn)(const void * c, void * acc);
typedef void (* foldl_fn)(void * acc, const void * c);

/**
 * Create a mappable wrapper for a function.
 */
#define WRAP_MAPPABLE(f, type, name)              \
	type * _mappable_ ## name(type * data)    \
	{                                         \
		*data = f(*data);                 \
		return data;                      \
	}                                         \
	map_fn name = (map_fn) _mappable_ ## name

/**
 * Create a wrapper for a function that can be used as a predicate.
 */
#define WRAP_PREDICATE(f, type, name)                \
	bool _predicate_ ## name(const type * data)  \
	{                                            \
		return f(*data);                     \
	}                                            \
	pred_fn name = (pred_fn) _predicate_ ## name

/**
 * Create a wrapper for a function that can be used as a comparator.
 */
#define WRAP_COMPARATOR(f, type, name)                            \
	bool _comparator_ ## name(const type * a, const type * b) \
	{                                                         \
		return f(*a, *b)                                  \
	}                                                         \
	comp_fn name = (comp_fn) _comparator_ ## name

/**
 * Create a right foldable wrapper for a function.
 */
#define WRAP_RFOLDABLE(f, type, name)                          \
	type * _rfoldable_ ## name(const type * a, type * acc) \
	{                                                      \
		*acc = f(*a, *acc);                            \
		return acc;                                    \
	}                                                      \
	foldr_fn name = (foldr_fn) _rfoldable_ ## name

/**
 * Create a left foldable wrapper for a function.
 */
#define WRAP_LFOLDABLE(f, type, name)                          \
	type * _lfoldable_ ## name(type * acc, const type * a) \
	{                                                      \
		*acc = f(*acc, *a);                            \
		return acc;                                    \
	}                                                      \
	foldl_fn name = (foldl_fn) _lfoldable_ ## name

#endif /* __HOF_H */
