/* ring_buffer.h - Ring Buffer API
 * Copyright (C) 2018 Quytelda Kahja
 *
 * This file is part of focs.
 *
 * focs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
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

#include "focs.h"
#include "hof.h"
#include "focs/data_structure.h"
#include "sync/rwlock.h"

START_DS(ring_buffer) {
	void * head;
	void * tail;

	void * data;
	size_t length;

	struct rwlock * rwlock;
} END_DS(ring_buffer);

#define __SPACE(buf)  (DS_DATA_SIZE(buf) * DS_ENTRIES(buf))
#define __LENGTH(buf) (DS_PRIV(buf)->length)
#define __HEAD(buf)   (DS_PRIV(buf)->head)
#define __TAIL(buf)   (DS_PRIV(buf)->tail)

#define __IS_EMPTY(buf)       (__LENGTH(buf) <= 0)
#define __IS_FULL(buf)        (__LENGTH(buf) >= DS_ENTRIES(buf))
#define __INDEX_ABS(buf, rel) (__IS_EMPTY(buf) ? 0 : mod(rel, __LENGTH(buf)))

static inline __pure __nonulls void * __index_to_addr(const ring_buffer buf,
	                                              const size_t index)
{
	size_t offset;
	size_t start;

	/* Doing arithmetic with void pointers is tricksy, even in GNU C.
	 * Cast all our pointers to size_t integers before doing arithmetic. */
	size_t data = (size_t) DS_PRIV(buf)->data;
	size_t head = (size_t) DS_PRIV(buf)->head;

	start = head - data;
	offset = index * DS_DATA_SIZE(buf);
	offset = (start + offset) % __SPACE(buf);
	return (void *) (data + offset);
}

static inline __pure __nonulls size_t __addr_to_index(const ring_buffer buf,
	                                              const void * addr)
{
	size_t offset;

	/* Doing arithmetic with void pointers is tricksy, even in GNU C.
	 * Cast all our pointers to size_t integers before doing arithmetic. */
	size_t head = (size_t) DS_PRIV(buf)->head;
	size_t mark = (size_t) addr;

	offset = mod((ssize_t) (mark - head), (ssize_t) __SPACE(buf));
	return offset / DS_DATA_SIZE(buf);
}

static inline __pure __nonulls void * __prev(const ring_buffer buf,
	                                     const void * addr)
{
	size_t start;
	size_t offset;

	/* Doing arithmetic with void pointers is tricksy, even in GNU C.
	 * Cast all our pointers to size_t integers before doing arithmetic. */
	size_t data = (size_t) DS_PRIV(buf)->data;
	size_t mark = (size_t) addr;

	start = mark - data;
	offset = mod((ssize_t) (start - DS_DATA_SIZE(buf)),
		     (ssize_t) DS_ENTRIES(buf));
	return (void *) (data + offset);
}

static inline __pure __nonulls void * __next(const ring_buffer buf,
	                                     const void * addr)
{
	size_t start;
	size_t offset;

	/* Doing arithmetic with void pointers is tricksy, even in GNU C.
	 * Cast all our pointers to size_t integers before doing arithmetic. */
	size_t data = (size_t) DS_PRIV(buf)->data;
	size_t mark = (size_t) addr;

	start = mark - data;
	offset = (start + DS_DATA_SIZE(buf)) % DS_ENTRIES(buf);
	return (void *) (data + offset);
}

/**
 * Advance through a ring buffer block by block.
 * @param list    The ring buffer to iterate over
 * @param current A pointer that will point to the current data block
 *
 * ring_buffer_foreach() should be used like a for loop; for example:
 * ```
 * struct data_type * current;
 * ring_buffer_foreach(buf, current) {
 *         do_something(current);
 * }
 * ```
 * The loop will work whether `current` is a void pointer or a pointer to a type
 * with the same size as (or theoretically less than) the buffer's data blocks.
 */
#define ring_buffer_foreach_i(buf, index, current)   \
	for(index = 0, current = __HEAD(buf);        \
	    index < __LENGTH(buf);                   \
	    index++, current = __next(buf, current))

#define ring_buffer_foreach(buf, current)       \
	size_t _i;                              \
	ring_buffer_foreach_i(buf, _i, current)

/**
 * Advance through a ring buffer block by block in reverse.
 * @param list    The ring buffer to iterate over
 * @param current A pointer that will point to the current data block
 *
 * The syntax of ring_buffer_foreach_rev() is the same as ring_buffer_foreach().
 */
#define ring_buffer_foreach_i_rev(buf, index, current) \
	for(index = 0, current = __TAIL(buf);          \
	    index < __LENGTH(buf);                     \
	    index++, current = __prev(buf, current))

#define ring_buffer_foreach_rev(buf, current)       \
	size_t _i;                                  \
	ring_buffer_foreach_i_rev(buf, _i, current)

/**
 * Create a new ring buffer with the given properties.
 * @param props A pointer to a data structure properties structure (non-NULL)
 *
 * Allocates and initializes a new ring buffer with the given properties.
 *
 * @return Upon successful completion, rb_create() shall return the newly
 * created ring buffer.  Otherwise, `NULL` shall be returned and `errno` set to
 * indicate the error.
 */
ring_buffer __nonulls rb_create(const struct ds_properties * props);

/**
 * Destroy and deallocate a ring buffer.
 * @param buf A pointer to a `struct ring_buffer` (non-NULL)
 *
 * Destroys and deallocates the ring buffer pointed to by `buf`.
 */
void __nonulls rb_destroy(ring_buffer * buf);

/**
 * Determine the number of data blocks stored in a ring buffer.
 * @param buf The ring buffer to check (non-NULL)
 *
 * @return This function shall return the number of data blocks stored in the
 * ring buffer `buf`.  This is distinct from the capacity of the ring buffer,
 * which is the number of entries it will contain when full.
 */
size_t __nonulls rb_size(const ring_buffer buf);

/**
 * Determine if a ring buffer is empty.
 * @param buf The ring buffer to check (non-NULL)
 *
 * @return This function shall return `true` if `buf` is empty, or `false`
 * otherwise.
 */
bool __nonulls rb_empty(const ring_buffer buf);

/**
 * Determine if a ring buffer is full.
 * @param buf The ring buffer to check (non-NULL)
 *
 * @return This function shall return `true` if `buf` is full, or `false`
 * otherwise.
 */
bool __nonulls rb_full(const ring_buffer buf);

/**
 * Push a new data block onto the head of a ring buffer.
 * @param buf The ring buffer to push onto
 * @param data A pointer to the data to push
 *
 * Push a newly allocated copy of `data` onto the head of `buf`.
 */
bool __nonulls rb_push_head(ring_buffer buf, const void * data);

/**
 * Push a new data block onto the tail of a ring buffer.
 * @param buf The ring buffer to push onto
 * @param data A pointer to the data to push
 *
 * Push a newly allocated copy of `data` onto the tail of `buf`.
 */
bool __nonulls rb_push_tail(ring_buffer buf, const void * data);

/**
 * Pop a data element from the head of a ring buffer.
 * @param buf The ring buffer to pop from (non-NULL)
 *
 * Remove and return the data element at the head of `buf`.  After this
 * operation the returned data element *will no longer be stored in* `buf`.
 *
 * @return Upon successful completion, this function shall return a pointer to
 * the data block stored at the head of `buf`; otherwise, `NULL` shall be
 * returned and `errno` set appropriately.
 *
 * This pointer must be explicitly freed with free() when it is no longer
 * needed.  It is **not** equivalent to the pointer which was used to insert the
 * data into `buf`.
 */
void * __nonulls rb_pop_head(ring_buffer buf);

/**
 * Pop a data element from the tail of a ring buffer.
 * @param buf The ring buffer to pop from (non-NULL)
 *
 * Remove and return the data block at the tail of `buf`.  After this
 * operation the returned data block *will no longer be stored in* `buf`.
 *
 * @return Upon successful completion, this function shall return a pointer to
 * the data block stored at the tail of `buf`; otherwise, `NULL` shall be
 * returned and `errno` set appropriately.
 *
 * This pointer must be explicitly freed with free() when it is no longer
 * needed.  It is **not** equivalent to the pointer which was used to insert the
 * data into `buf`.
 */
void * __nonulls rb_pop_tail(ring_buffer buf);

/**
 * Insert a data block into a ring buffer at a certain position.
 * @param buf  The ring buffer to insert into (non-NULL)
 * @param data A pointer to the data to insert
 * @param pos  The position to insert the data block at
 *
 * Insert a newly allocated copy of `data` into `buf` at the index indicated
 * by `pos`.
 *
 * @return Upon successful completion, this function shall return `true`;
 * otherwise, `false` shall be returned and `errno` set appropriately.
 */
bool __nonulls rb_insert(ring_buffer buf, const void * data, const ssize_t pos);

/**
 * Fetch a data block from a given index of a ring buffer.
 * @param buf The ring buffer to fetch from
 * @param pos The index to fetch the block from
 *
 * Fetch the data stored at index `pos` in `buf`.
 *
 * @return A pointer to the data at index `pos`, or `NULL` on failure.
 * This pointer should **not** be free()d explicitly, or the `buf` will become
 * corrupted.  This pointer will be free()d when rb_free() is called, so if
 * the data is needed after `buf` is destroyed, make a copy of it or make
 * sure to call rb_remove() on the data's index before destroying `buf`.
 */
void * __nonulls rb_fetch(const ring_buffer buf, const ssize_t pos);

/**
 * Delete a data element from a given index of a ring buffer.
 * @param list The ring buffer to delete from
 * @param pos  The index to delete the element at
 *
 * Delete the copy of `data` stored in `buf` at the index indicated by `pos`.
 *
 * @return Upon successful completion, this function shall return `true`;
 * otherwise, `false` shall be returned and `errno` set appropriately..
 */
bool __nonulls rb_delete(ring_buffer buf, const size_t pos);

/**
 * Delete and return a data element from a given index of a ring buffer.
 * @param list The ring buffer to delete from
 * @param pos  The index to delete the element at
 *
 * Delete the copy of `data` stored in `buf` at the index indicated by `pos`.
 *
 * @return A pointer to the data removed from `buf`, or `NULL` on failure.
 * This pointer must be explicitly freed with free() when it is no longer
 * needed.  It is **not** equivalent to the pointer which was used to insert
 * the data into `buf`.
 */
void * __nonulls rb_remove(ring_buffer buf, const size_t pos);

/* ############################ *
 * # Transformation Functions # *
 * ############################ */

/**
 * Map a function over the contents of a ring buffer in-place.
 * @param buf A ring buffer to map over
 * @param fn A function that will transform each data block in the buffer
 *
 * A map operation iterates over `buf` and transforms each data block using
 * the function `fn`, replacing the old value with the result of the
 * transformation.  In pseudo-code:
 * ```
 * for i from 0 to (length - 1):
 * 	buffer[i] = fn(buffer[i])
 * ```
 */
void __nonulls rb_map(const ring_buffer buf, const map_fn fn);

/**
 * Right associative fold for ring buffers.
 * @param buf  A ring buffer to reduce
 * @param fn   A binary function that will sequentially reduce values
 * @param init An initial value for the fold
 *
 * A right associative fold uses the binary function `fn` to sequentially reduce
 * a list of values to a single value, starting from some initial value `init`:
 * ```
 * fn(init, fn(buf[0], fn(buf[1], ...)))
 * ```
 *
 * @return Upon successful completetion, this function shall return the result
 * of a right associate fold over `buf`.  If `buf` is empty, the fold will be
 * equal to the value of `buf`.  Otherwise, `NULL` shall be returned and errno
 * set to indicate the error.
 */
void * __nonulls rb_foldr(const ring_buffer buf,
	                  const foldr_fn fn,
	                  const void * init);

/**
 * Left associative fold for ring buffers.
 * @param buf  A ring buffer to reduce
 * @param fn   A binary function that will sequentially reduce values
 * @param init An initial value for the fold
 *
 * A left associative fold uses the binary function `fn` to sequentially reduce
 * a list of values to a single value, starting from some initial value `init`:
 * ```
 * fn(fn(fn(..., init), buf[0]), buf[1])
 * ```
 *
 * @return Upon successful completetion, this function shall return the result
 * of a left associate fold over `buf`.  If `buf` is empty, the fold will be
 * equal to the value of `buf`.  Otherwise, `NULL` shall be returned and errno
 * set to indicate the error.
 */
void * __nonulls rb_foldl(const ring_buffer buf,
	                  const foldl_fn fn,
	                  const void * init);

#ifdef DEBUG
#include <stdio.h>

/**
 * Dump the contents of a ring_buffer to standard output.
 * @param buf The buffer to display.
 *
 * Prints the contents of a ring buffer in nice format, byte by byte, including
 * information on addresses, indices, and the locations of the head and tail
 * pointers.
 */
void rb_dump(ring_buffer buf);
#endif /* DEBUG */

#ifdef GENERICS

static const struct mgmt_operations mgmt_ops = {
	.empty   = (empty_mgmt_fn)    rb_empty,
	.size    = (size_mgmt_fn)    rb_size,
	.destroy = (destroy_mgmt_fn) rb_destroy,
};

static const struct hof_operations hof_ops = {
};

#else /* GENERICS */

static const __unused void * mgmt_ops = NULL;
static const __unused void * hof_ops  = NULL;

#endif /* GENERICS */
