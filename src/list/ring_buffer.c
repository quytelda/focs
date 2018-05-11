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

#define INDEX_ABS(relative) mod(relative, (ssize_t) DS_PRIV(buf)->length)

static inline __pure size_t __length(const ring_buffer buf)
{
	return DS_PRIV(buf)->length;
}

static inline __pure size_t __space(const ring_buffer buf)
{
	return DS_DATA_SIZE(buf) * DS_ENTRIES(buf);
}

static inline __pure bool __is_empty(const ring_buffer buf)
{
	return (__length(buf) <= 0);
}

static inline __pure bool __is_full(const ring_buffer buf)
{
	return (__length(buf) >= DS_ENTRIES(buf));
}

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
	offset = (start + offset) % __space(buf);
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

	offset = mod((ssize_t) (mark - head), (ssize_t) __space(buf));
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

static __nonulls bool __push_head(ring_buffer buf, const void * data)
{
	if(!DS_OVERWRITE(buf) && __is_full(buf))
		return_with_errno(ENOBUFS, false);

	DS_PRIV(buf)->head = __prev(buf, DS_PRIV(buf)->head);
	memcpy(DS_PRIV(buf)->head, data, DS_DATA_SIZE(buf));
	DS_PRIV(buf)->length = MIN(DS_PRIV(buf)->length + 1, DS_ENTRIES(buf));

	return true;
}

static __nonulls bool __push_tail(ring_buffer buf, const void * data)
{
	if(!DS_OVERWRITE(buf) && __is_full(buf))
		return_with_errno(ENOBUFS, false);

	memcpy(DS_PRIV(buf)->tail, data, DS_DATA_SIZE(buf));
	DS_PRIV(buf)->tail = __next(buf, DS_PRIV(buf)->tail);
	DS_PRIV(buf)->length = MIN(DS_PRIV(buf)->length + 1, DS_ENTRIES(buf));

	return true;
}

static __nonulls void * __pop_head(ring_buffer buf)
{
	void * data;

	if(__is_empty(buf))
		return_with_errno(EFAULT, NULL);

	data = malloc(DS_DATA_SIZE(buf));
	if(!data)
		return_with_errno(ENOMEM, NULL);

	memcpy(data, DS_PRIV(buf)->head, DS_DATA_SIZE(buf));
	DS_PRIV(buf)->head = __next(buf, DS_PRIV(buf)->head);
	DS_PRIV(buf)->length--;

	return data;
}

static __nonulls void * __pop_tail(ring_buffer buf)
{
	void * data;

	if(__is_empty(buf))
		return_with_errno(EFAULT, NULL);

	data = malloc(DS_DATA_SIZE(buf));
	if(!data)
		return_with_errno(ENOMEM, NULL);

	DS_PRIV(buf)->tail = __prev(buf, DS_PRIV(buf)->tail);
	memcpy(data, DS_PRIV(buf)->tail, DS_DATA_SIZE(buf));
	DS_PRIV(buf)->length--;

	return data;
}

static void * __shift_forward(const ring_buffer buf,
			      const size_t start,
			      const size_t end)
{
	void * dest;
	void * front;
	void * src;

	front = dest = __prev(buf, __index_to_addr(buf, start));
	for(ssize_t i = start; i <= end; i++) {
		src = __index_to_addr(buf, i);
		memcpy(dest, src, DS_DATA_SIZE(buf));
		dest = src;
	}

	return front;
}

static void * __shift_backward(const ring_buffer buf,
			       const size_t start,
			       const size_t end)
{
	void * back;
	void * dest;
	void * src;

	back = dest = __next(buf, __index_to_addr(buf, end));
	for(ssize_t i = end; i >= start; i--) {
		src = __index_to_addr(buf, i);
		memcpy(dest, src, DS_DATA_SIZE(buf));
		dest = src;
	}

	return back;
}

static void __open_gap(ring_buffer buf,
	               const size_t index)
{
	size_t end;
	void * mark;

	end = DS_PRIV(buf)->length - 1;
	if(index <= (end - index)) {
		mark = __shift_forward(buf, 0, index);
		DS_PRIV(buf)->head = mark;
	} else {
		mark = __shift_backward(buf, index, end);
		DS_PRIV(buf)->tail = __next(buf, mark);
	}

	(DS_PRIV(buf)->length)++;
}

static __nonulls bool __insert(ring_buffer buf,
			       const void * data,
			       const ssize_t relative)
{
	size_t absolute;
	void * addr;

	if(!DS_OVERWRITE(buf) && __is_full(buf))
		return_with_errno(ENOBUFS, false);

	absolute = INDEX_ABS(relative);
	if(!DS_OVERWRITE(buf))
		__open_gap(buf, absolute);

	addr = __index_to_addr(buf, absolute);
	memcpy(addr, data, DS_DATA_SIZE(buf));

	return true;
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
	/* Destroy the private data section. */
	free_null(DS_PRIV(*buf)->data);
	rwlock_free(&DS_PRIV(*buf)->rwlock);

	/* Deallocate the data structure. */
	DS_FREE(buf);
}

size_t rb_size(const ring_buffer buf)
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

bool rb_full(const ring_buffer buf)
{
	bool success;

	rwlock_reader_entry(DS_PRIV(buf)->rwlock);
	success = __is_full(buf);
	rwlock_reader_exit(DS_PRIV(buf)->rwlock);

	return success;
}

bool rb_push_head(ring_buffer buf, const void * data)
{
	bool success;

	rwlock_writer_entry(DS_PRIV(buf)->rwlock);
	success = __push_head(buf, data);
	rwlock_writer_exit(DS_PRIV(buf)->rwlock);

	return success;
}

bool rb_push_tail(ring_buffer buf, const void * data)
{
	bool success;

	rwlock_writer_entry(DS_PRIV(buf)->rwlock);
	success = __push_tail(buf, data);
	rwlock_writer_exit(DS_PRIV(buf)->rwlock);

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

bool rb_insert(ring_buffer buf, const void * data, const ssize_t pos)
{
	bool success;

	rwlock_writer_entry(DS_PRIV(buf)->rwlock);
	success = __insert(buf, data, pos);
	rwlock_writer_exit(DS_PRIV(buf)->rwlock);

	return success;
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

	for(size_t i = 0; i < DS_ENTRIES(buf); i++) {
		uint8_t * addr;
		size_t pos;

		addr = ((uint8_t *) priv->data) + i;
		pos  = __addr_to_index(buf, addr);

		printf("%p [%lu]: %#04x", addr, pos, *addr);

		if(addr == priv->data)
			printf(" <data>");
		if(addr == priv->head)
			printf(" (head)");
		if(addr == priv->tail)
			printf(" (tail)");

		putchar('\n');
	}
}

#endif /* DEBUG */
