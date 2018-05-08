/* ring_buffer.c - Ring Buffer Implementation
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

#include "list/ring_buffer.h"
#include "sync/rwlock.h"

__attribute__((pure))
static inline size_t __length(const ring_buffer buf)
{
	return DS_PRIV(buf)->length;
}

__attribute__((pure))
static inline size_t __space(const ring_buffer buf)
{
	return DS_PROPS(buf)->data_size * DS_PROPS(buf)->entries;
}

__attribute__((pure))
static inline bool __is_empty(const ring_buffer buf)
{
	return (__length(buf) <= 0);
}

__attribute__((pure))
static inline bool __is_full(const ring_buffer buf)
{
	return (__length(buf) >= DS_PROPS(buf)->entries);
}

__attribute__((pure))
static inline bool __index_OOB(const ring_buffer buf,
			       const ssize_t pos,
			       const bool inclusive)
{
	if(inclusive)
		return ((pos < -1) || (pos > __length(buf)));

	return ((pos < 0) || (pos >= __length(buf)));
}

__attribute__((pure))
static inline void * __rbpos_to_addr(const ring_buffer buf,
				     const ssize_t pos)
{
	size_t offset;

	/* ISO C99 doesn't allow arithmetic on void pointers,
	 * so cast all pointers to size_t integers for arithmetic. */
	const size_t head  = (size_t) DS_PRIV(buf)->head;
	const size_t start = (size_t) DS_PRIV(buf)->data;

	offset = head - start;
	offset += mod((ssize_t) DS_PROPS(buf)->data_size * pos,
		      (ssize_t) DS_PROPS(buf)->entries);
	offset %= __space(buf);

	return (void *) (start + offset);
}

__attribute__((pure))
static inline ssize_t __addr_to_rbpos(const ring_buffer buf,
				      const void * addr)
{
	ssize_t distance;
	ssize_t pos;

	/* ISO C99 doesn't allow arithmetic on void pointers,
	 * so cast all pointers to size_t integers for arithmetic. */
	const size_t mark = (size_t) addr;
	const size_t head = (size_t) DS_PRIV(buf)->head;

	distance = mark - head;
	pos = distance / DS_PROPS(buf)->data_size;

	/* The address must be aligned to the beginning of the block.
	 * Truncated division is floored division for positive results (good),
	 * but for negative numbers it is division always rounded up (bad).
	 * So, to align negative results we may need to subtract one. */
	if((pos < 0) && !aligned(distance, DS_PROPS(buf)->data_size, 0))
		pos -= 1;

	return pos;
}

static inline void * __write(const ring_buffer buf,
			     const void * data,
			     const ssize_t pos)
{
	void * addr;

	addr = __rbpos_to_addr(buf, pos);
	memcpy(addr, data, DS_PROPS(buf)->data_size);

	return addr;
}

static inline void * __read(const ring_buffer buf,
			    void * data,
			    const ssize_t pos)
{
	void * addr;

	addr = __rbpos_to_addr(buf, pos);
	memcpy(data, addr, DS_PROPS(buf)->data_size);

	return addr;
}

static void * __pop_head(ring_buffer buf)
{
	void * data;

	if(__is_empty(buf))
		return_with_errno(EFAULT, NULL);

	data = malloc(DS_PROPS(buf)->data_size);
	if(!data)
		return_with_errno(ENOMEM, NULL);

	__read(buf, data, 0);

	DS_PRIV(buf)->head = __rbpos_to_addr(buf, 1);
	DS_PRIV(buf)->length--;

	return data;
}

static void * __pop_tail(ring_buffer buf)
{
	void * data;

	if(__is_empty(buf))
		return_with_errno(EFAULT, NULL);

	data = malloc(DS_PROPS(buf)->data_size);
	if(!data)
		return_with_errno(ENOMEM, NULL);

	DS_PRIV(buf)->tail = __read(buf, data, DS_PRIV(buf)->length - 1);
	DS_PRIV(buf)->length--;

	return data;
}

static bool __push_head(ring_buffer buf,
			const void * data,
			const bool overwrite)
{
	if(!overwrite && __is_full(buf))
		return_with_errno(ENOBUFS, false);

	DS_PRIV(buf)->head = __write(buf, data, -1);
	DS_PRIV(buf)->length = MIN(DS_PRIV(buf)->length + 1,
				   DS_PROPS(buf)->entries);
	return true;
}

static bool __push_tail(ring_buffer buf,
			const void * data,
			const bool overwrite)
{
	if(!overwrite && __is_full(buf))
		return_with_errno(ENOBUFS, false);

	__write(buf, data, DS_PRIV(buf)->length);
	DS_PRIV(buf)->length = MIN(DS_PRIV(buf)->length + 1,
				   DS_PROPS(buf)->entries);
	DS_PRIV(buf)->tail = __rbpos_to_addr(buf, DS_PRIV(buf)->length);
	return true;
}

static void * __shift_forward(const ring_buffer buf,
			      const ssize_t start,
			      const ssize_t end)
{
	void * dest;
	void * front;
	void * src;

	front = dest = __rbpos_to_addr(buf, start - 1);
	for(ssize_t i = start; i <= end; i++) {
		src = __rbpos_to_addr(buf, i);
		memcpy(dest, src, DS_PROPS(buf)->data_size);
		dest = src;
	}

	return front;
}

static void * __shift_backward(const ring_buffer buf,
			       const ssize_t start,
			       const ssize_t end)
{
	void * back;
	void * dest;
	void * src;

	back = dest = __rbpos_to_addr(buf, end + 1);
	for(ssize_t i = end; i >= start; i--) {
		src = __rbpos_to_addr(buf, i);
		memcpy(dest, src, DS_PROPS(buf)->data_size);
		dest = src;
	}

	return back;
}

static inline void __open_gap(ring_buffer buf,
			      const ssize_t pos)
{
	size_t last;

	last = DS_PRIV(buf)->length - 1;
	if(pos < (last - pos))
		DS_PRIV(buf)->head = __shift_forward(buf, 0, pos);
	else
		DS_PRIV(buf)->tail = __shift_backward(buf, pos, last);

	(DS_PRIV(buf)->length)++;
}

static inline void __close_gap(ring_buffer buf,
			       const ssize_t pos)
{
	size_t last;

	last = DS_PRIV(buf)->length - 1;
	if(pos < (last - pos)) {
		__shift_backward(buf, 0, pos - 1);
		DS_PRIV(buf)->head = __rbpos_to_addr(buf, 1);
	} else {
		__shift_forward(buf, pos + 1, last);
		DS_PRIV(buf)->tail = __rbpos_to_addr(buf, last);
	}

	(DS_PRIV(buf)->length)--;
}

static bool __insert(ring_buffer buf,
		     const void * data,
		     const ssize_t pos,
		     const bool overwrite)
{
	if(__index_OOB(buf, pos, true))
		return_with_errno(EFAULT, false);

	if(!overwrite && __is_full(buf))
		return_with_errno(ENOBUFS, false);

	/* If overwrite is disabled, open a gap at `pos` wide enough to fit a
	 * new data block so none of the old data is overwritten. */
	if(!overwrite)
		__open_gap(buf, pos);

	__write(buf, data, pos);
	return true;
}

static void * __remove(ring_buffer buf,
		       const ssize_t pos,
		       const bool overwrite)
{
	void * data;

	if(__index_OOB(buf, pos, true))
		return_with_errno(EFAULT, NULL);

	data = malloc(DS_PROPS(buf)->data_size);
	if(!data)
		return_with_errno(ENOMEM, NULL);

	__read(buf, data, pos);

	/* If overwrite is disabled, close the newly empty gap at `pos`.
	 * This is both to make space for new data blocks at the head/tail, and
	 * so this function behaves as the inverse of __insert(). */
	if(!overwrite)
		__close_gap(buf, pos);
	else
		__zero(buf, pos);
	return data;
}

static bool __delete(ring_buffer buf,
		     const ssize_t pos,
		     const bool overwrite)
{
	if(__index_OOB(buf, pos, true))
		return_with_errno(EFAULT, false);

	/* If overwrite is disabled, close the newly empty gap at `pos`.
	 * This is both to make space for new data blocks at the head/tail, and
	 * so this function behaves as the inverse of __insert(). */
	if(!overwrite)
		__close_gap(buf, pos);
	else
		__zero(buf, pos);

	return true;
}

static void * __fetch(ring_buffer buffer,
		      const ssize_t pos)
{
	void * data;

	if(__is_empty(buffer) || __index_OOB(buffer, pos, false))
		return_with_errno(EFAULT, NULL);

	data = malloc(DS_PROPS(buffer)->data_size);
	if(!data)
		return_with_errno(ENOMEM, NULL);

	__read(buffer, data, pos);
	return data;
}

ring_buffer rb_create(const struct ds_properties * props)
{
	ring_buffer buf;
	struct ring_buffer_priv * priv;

	DS_ALLOC(buf);
	if(!buf)
		return_with_errno(ENOMEM, NULL);

	DS_INIT(buf, props, &mgmt_ops, &hof_ops);

	/* Set up private data section. */
	priv = DS_PRIV(buf);
	priv->data = malloc(__space(buf));
	if(!priv->data)
		goto_with_errno(ENOMEM, exit);

	priv->head = priv->data;
	priv->tail = priv->data;
	priv->length = 0;

	if(rwlock_alloc(&priv->rwlock) < 0)
		goto_with_errno(errno, exit);

	return buf;

exit:
	rb_destroy(&buf);
	return NULL;
}

void rb_destroy(ring_buffer * buf)
{
	struct ring_buffer_priv * priv = DS_PRIV(*buf);

	/* Destroy the private data section. */
	priv->head = NULL;
	priv->tail = NULL;
	priv->length = 0;
	free_null(priv->data);

	/* TODO: destroy rwlock */

	/* Deallocate the data structure. */
	DS_FREE(buf);
}

size_t rb_size(ring_buffer buf)
{
	size_t size;

	rwlock_reader_entry(DS_PRIV(buf)->rwlock);
	size = __length(buf);
	rwlock_reader_exit(DS_PRIV(buf)->rwlock);

	return size;
}

bool rb_empty(const ring_buffer buf)
{
	bool success;

	rwlock_reader_entry(DS_PRIV(buf)->rwlock);
	success = __is_empty(buf);
	rwlock_reader_exit(DS_PRIV(buf)->rwlock);

	return success;
}

void * rb_pop_head(ring_buffer buf)
{
	void * data;

	rwlock_writer_entry(DS_PRIV(buf)->rwlock);
	data = __pop_head(buf);
	rwlock_writer_exit(DS_PRIV(buf)->rwlock);

	return data;
}

void * rb_pop_tail(ring_buffer buf)
{
	void * data;

	rwlock_writer_entry(DS_PRIV(buf)->rwlock);
	data = __pop_tail(buf);
	rwlock_writer_exit(DS_PRIV(buf)->rwlock);

	return data;
}

bool rb_push_head(ring_buffer buf, const void * data)
{
	bool success;

	rwlock_writer_entry(DS_PRIV(buf)->rwlock);
	success = __push_head(buf, data, DS_PROPS(buf)->overwrite);
	rwlock_writer_exit(DS_PRIV(buf)->rwlock);

	return success;
}

bool rb_push_tail(ring_buffer buf, const void * data)
{
	bool success;

	rwlock_writer_entry(DS_PRIV(buf)->rwlock);
	success = __push_tail(buf, data, DS_PROPS(buf)->overwrite);
	rwlock_writer_exit(DS_PRIV(buf)->rwlock);

	return success;
}

bool rb_insert(ring_buffer buf,
	       const void * data,
	       const ssize_t pos)
{
	bool success;

	rwlock_writer_entry(DS_PRIV(buf)->rwlock);
	success = __insert(buf, data, pos, DS_PROPS(buf)->overwrite);
	rwlock_writer_exit(DS_PRIV(buf)->rwlock);

	return success;
}

void * rb_fetch(ring_buffer buf,
		const ssize_t pos)
{
	void * data;

	rwlock_writer_entry(DS_PRIV(buf)->rwlock);
	data = __fetch(buf, pos);
	rwlock_writer_exit(DS_PRIV(buf)->rwlock);

	return data;
}

#ifdef DEBUG

void rb_dump(ring_buffer buf)
{
	struct ring_buffer_priv * priv;

	if(!buf) {
		printf("`buf` is unallocated/unitialized (NULL).\n");
		return;
	}

	priv = DS_PRIV(buf);
	printf("Buffer length: %ld", priv->length);

	if(__is_empty(buf))
		puts(" (empty)\n");
	else if(__is_full(buf))
		puts(" (full)\n");
	else
		puts("\n");

	for(size_t i = 0; i < DS_PROPS(buf)->entries; i++) {
		uint8_t * addr;
		ssize_t pos;

		addr = ((uint8_t *) priv->data) + i;
		pos  = __addr_to_rbpos(buf, addr);

		printf("%p (%ld): %#04x", addr, pos, *addr);

		if(addr == priv->data)
			printf(" (start)");
		if(addr == priv->head)
			printf(" (head)");
		if(addr == priv->tail)
			printf(" (tail)");

		putchar('\n');
	}
}

#endif /* DEBUG */
