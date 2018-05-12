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
#include "focs/data_structure.h"
#include "sync/rwlock.h"

START_DS(ring_buffer) {
	void * head;
	void * tail;

	void * data;
	size_t length;

	struct rwlock * rwlock;
} END_DS(ring_buffer);

/**
 * Create a new doubly ring buffer with the given properties.
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
 * which is the number of entries it may contain when full.
 */
size_t __nonulls rb_size(const ring_buffer buf);

/**
 * Determine if a ring buffer is empty.
 * @param buf The ring buffer to check (non-NULL)
 *
 * @return This function shall return `true` if `buf` is empty,
 * or `false` otherwise.
 */
bool __nonulls rb_empty(const ring_buffer buf);

/**
 * Determine if a ring buffer is full.
 * @param buf The ring buffer to check (non-NULL)
 *
 * @return This function shall return `true` if `buf` is full,
 * or `false` otherwise.
 */
bool __nonulls rb_full(const ring_buffer buf);

/**
 * Push a new data block onto the head of a ring buffer.
 * @param buf The ring buffer to push onto
 * @param data A pointer to the data to push
 *
 * Push a newly allocated copy of `data` onto the head of `buf`.
 */
bool __nonulls rb_push_head(ring_buffer buf,
	                    const void * data);

/**
 * Push a new data block onto the tail of a ring buffer.
 * @param buf The ring buffer to push onto
 * @param data A pointer to the data to push
 *
 * Push a newly allocated copy of `data` onto the tail of `buf`.
 */
bool __nonulls rb_push_tail(ring_buffer buf,
	                    const void * data);

/**
 * Pop a data element from the head of a ring buffer.
 * @param buf The ring buffer to pop from (non-NULL)
 *
 * Remove and return the data element at the head of `buf`.  After this
 * operation the returned data element *will no longer be stored in* `buf`.
 *
 * @return Upon successful completion, this function shall returns a pointer to
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
 * @return Upon successful completion, this function shall returns a pointer to
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
 *             (must be an index in the range `0..length-1`)
 *
 * Insert a newly allocated copy of `data` into `buf` at the index indicated
 * by `pos`.
 *
 * @return Upon successful completion, this function shall return `true`;
 * otherwise, `false` shall be returned and `errno` set appropriately.
 */
bool __nonulls rb_insert(ring_buffer buf,
	                 const void * data,
	                 const ssize_t pos);

/**
 * Fetch a data block from a given index of a ring buffer.
 * @param buf The ring buffer to fetch from
 * @param pos The index to fetch the block from
 *             (must be an index in the range `0..length - 1`)
 *
 * Fetch the data stored at index `pos` in `buf`.
 *
 * @return A pointer to the data at index `pos`, or `NULL` on failure.
 * This pointer should **not** be free()d explicitly, or the `buf` will become
 * corrupted.  This pointer will be free()d when dl_free() is called, so if
 * the data is needed after `buf` is destroyed, make a copy of it, or make
 * sure to call rb_remove() on the data's index before destroying `buf`.
 */
void * __nonulls rb_fetch(const ring_buffer buf, const ssize_t pos);

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
