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

static inline void * __rbpos_to_addr(const struct ring_buffer * buf,
				     const ssize_t pos)
{
	size_t roff;
	size_t space;
	size_t offset;

	/* ISO C99 doesn't allow arithmetic on void pointers,
	 * so cast all pointers to size_t integers for arithmetic. */
	const size_t start = (size_t) buf->data;
	const size_t head  = (size_t) buf->head;

	offset = mod((ssize_t) DS_DATA_SIZE(buf) * pos, DS_ENTRIES(buf));

	roff  = (head - start) + offset;
	space = DS_DATA_SIZE(buf) * DS_ENTRIES(buf);
	return (void *) (start + (roff % space));
}

static inline ssize_t __addr_to_rbpos(const struct ring_buffer * buf,
				      const void * vm_addr)
{
	ssize_t pos;
	ssize_t distance;

	/* ISO C99 doesn't allow arithmetic on void pointers,
	 * so cast all pointers to size_t integers for arithmetic. */
	const size_t head  = (size_t) buf->head;
	const size_t addr  = (size_t) vm_addr;

	distance = addr - head;
	pos = distance / DS_DATA_SIZE(buf);

	/* The address must be aligned to the beginning of the block.
	 * Truncation of integer division is equivelent to floored division for
	 * positive numbers (fine), but for negative numbers, it is equivelent
	 * to ceiling division, so we must subtract one to align to the
	 * beginning of the block. */
	if((distance < 0) && (distance % DS_DATA_SIZE(buf) != 0))
		pos--;

	return pos;
}

static inline bool __is_null(const struct ring_buffer * buf)
{
	return (buf->length == 0);
}

static inline bool __is_full(const struct ring_buffer * buf)
{
	return (buf->length >= DS_ENTRIES(buf));
}

static inline bool __index_OOB(const struct ring_buffer * buf,
			     const ssize_t pos,
			     const bool inclusive)
{
	if(inclusive)
		return ((pos < -1) || (pos > buf->length));

	return ((pos < 0) || (pos >= buf->length));
}

static inline void * __write(struct ring_buffer * buf,
			     const void * data,
			     const ssize_t pos)
{
	void * addr;

	addr = __rbpos_to_addr(buf, pos);
	memcpy(addr, data, DS_DATA_SIZE(buf));

	return addr;
}

static inline void * __read(struct ring_buffer * buf,
			    void * data,
			    const ssize_t pos)
{
	void * addr;

	addr = __rbpos_to_addr(buf, pos);
	memcpy(data, addr, DS_DATA_SIZE(buf));

	return addr;
}

static void * __pop_head(struct ring_buffer * buf)
{
	void * data;

	if(__is_null(buf))
		return_with_errno(EFAULT, NULL);

	data = malloc(DS_DATA_SIZE(buf));
	if(!data)
		return_with_errno(ENOMEM, NULL);

	__read(buf, data, 0);

	buf->head = __rbpos_to_addr(buf, 1);
	(buf->length)--;

	return data;
}

static void * __pop_tail(struct ring_buffer * buf)
{
	void * data;

	if(__is_null(buf))
		return_with_errno(EFAULT, NULL);

	data = malloc(DS_DATA_SIZE(buf));
	if(!data)
		return_with_errno(ENOMEM, NULL);

	buf->tail = __read(buf, data, buf->length - 1);
	(buf->length)--;

	return data;
}

static bool __push_head(struct ring_buffer * buf,
			const void * data,
			const bool overwrite)
{
	if(__is_full(buf)) {
		if(overwrite)
			free(__pop_tail(buf));
		else
			return_with_errno(ENOMEM, false);
	}

	buf->head = __write(buf, data, -1);
	(buf->length)++;

	return true;
}

static bool __push_tail(struct ring_buffer * buf,
			const void * data,
			const bool overwrite)
{
	if(__is_full(buf)) {
		if(overwrite)
			free(__pop_head(buf));
		else
			return_with_errno(ENOMEM, false);
	}

	__write(buf, data, buf->length);
	buf->tail = __rbpos_to_addr(buf, buf->length + 1);
	(buf->length)++;

	return true;
}

static void * __shift_forward(struct ring_buffer * buf,
			      const ssize_t start,
			      const ssize_t end)
{
	void * src;
	void * dest;
	void * front;

	(buf->length)++;

	front = dest = __rbpos_to_addr(buf, start - 1);
	for(ssize_t i = start; i <= end; i++) {
		src = __rbpos_to_addr(buf, i);
		memcpy(dest, src, DS_DATA_SIZE(buf));
		dest = src;
	}

	return front;
}

static void * __shift_backward(struct ring_buffer * buf,
			       const ssize_t start,
			       const ssize_t end)
{
	void * src;
	void * dest;
	void * back;

	(buf->length)++;

	back = dest = __rbpos_to_addr(buf, end + 1);
	for(ssize_t i = end; i >= start; i--) {
		src = __rbpos_to_addr(buf, i);
		memcpy(dest, src, DS_DATA_SIZE(buf));
		dest = src;
	}

	return back;
}

static bool __insert(struct ring_buffer * buf,
		     const void * data,
		     const ssize_t pos,
		     const bool overwrite)
{
	if(__index_OOB(buf, pos, true))
		return_with_errno(EFAULT, false);

	if(!overwrite && __is_full(buf))
		return_with_errno(ENOMEM, false);

	/* If overwrite is disabled, we want to shift all the contents of the
	 * buffer forward/backward by one so that we can insert the new element
	 * into a blank slot. */
	if(!overwrite) {
		if(pos <= (buf->length - pos))
			buf->head = __shift_forward(buf, 0, pos);
		else
			buf->tail = __shift_backward(buf, pos, buf->length - 1);
	}

	__write(buf, data, pos);

	return true;
}

static void * __fetch(struct ring_buffer * buf,
		      const ssize_t pos)
{
	void * data;

	if(__is_null(buf) || __index_OOB(buf, pos, false))
		return_with_errno(EFAULT, NULL);

	data = malloc(DS_DATA_SIZE(buf));
	if(!data)
		return_with_errno(ENOMEM, NULL);

	__read(buf, data, pos);

	return data;
}

int rb_alloc(struct ring_buffer ** buf,
	     const struct data_properties * props)
{
	*buf = malloc(sizeof(**buf));
	if(!*buf)
		goto_with_errno(ENOMEM, exit);

	DS_SET_PROPS(*buf, props);

	(*buf)->data = malloc(DS_ENTRIES(*buf) * DS_DATA_SIZE(*buf));
	if(!(*buf)->data)
		goto_with_errno(ENOMEM, exit);

	(*buf)->head = (*buf)->data;
	(*buf)->tail = (*buf)->data;
	(*buf)->length = 0;

	if(rwlock_alloc(&(*buf)->rwlock) < 0)
		goto exit;

	return 0;

exit:
	if(*buf) {
		if((*buf)->data)
			free((*buf)->data);

		if((*buf)->rwlock)
			rwlock_free(&(*buf)->rwlock);

		free(*buf);
	}

	return -1;
}

void rb_free(struct ring_buffer ** buf)
{
	rwlock_writer_entry((*buf)->rwlock);

	(*buf)->length = 0;
	(*buf)->head = NULL;
	(*buf)->tail = NULL;
	free((*buf)->data);

	rwlock_writer_exit((*buf)->rwlock);
	rwlock_free(&(*buf)->rwlock);

	free(*buf);
}

bool rb_null(struct ring_buffer * buf)
{
	bool success;

	rwlock_reader_entry(buf->rwlock);
	success = __is_null(buf);
	rwlock_reader_exit(buf->rwlock);

	return success;
}

bool rb_push_head(struct ring_buffer * buf, const void * data)
{
	bool success;

	rwlock_writer_entry(buf->rwlock);
	success = __push_head(buf, data, false);
	rwlock_writer_exit(buf->rwlock);

	return success;
}

bool rb_push_tail(struct ring_buffer * buf, const void * data)
{
	bool success;

	rwlock_writer_entry(buf->rwlock);
	success = __push_tail(buf, data, false);
	rwlock_writer_exit(buf->rwlock);

	return success;
}

void * rb_pop_head(struct ring_buffer * buf)
{
	void * data;

	rwlock_writer_entry(buf->rwlock);
	data = __pop_head(buf);
	rwlock_writer_exit(buf->rwlock);

	return data;
}

void * rb_pop_tail(struct ring_buffer * buf)
{
	void * data;

	rwlock_writer_entry(buf->rwlock);
	data = __pop_tail(buf);
	rwlock_writer_exit(buf->rwlock);

	return data;
}

bool rb_insert(struct ring_buffer * buf,
	       const void * data,
	       const ssize_t pos)
{
	bool success;

	rwlock_writer_entry(buf->rwlock);
	success = __insert(buf, data, pos, false);
	rwlock_writer_exit(buf->rwlock);

	return success;
}

void * rb_fetch(struct ring_buffer * buf,
		const ssize_t pos)
{
	void * data;

	rwlock_writer_entry(buf->rwlock);
	data = __fetch(buf, pos);
	rwlock_writer_exit(buf->rwlock);

	return data;
}

#ifdef DEBUG
#include <stdio.h>

void rb_show(struct ring_buffer * buf)
{
	printf("Buffer length: %ld", buf->length);

	if(__is_null(buf))
		puts(" (empty)\n");
	else if(__is_full(buf))
		puts(" (full)\n");
	else
		puts("\n");

	for(size_t i = 0; i < DS_ENTRIES(buf); i++) {
		ssize_t pos;
		uint8_t * vm_addr;

		vm_addr = ((uint8_t *) buf->data) + i;
		pos = __addr_to_rbpos(buf, vm_addr);

		printf("%p (%ld): %#04x", vm_addr, ABS_POS(buf, pos), *vm_addr);

		if(vm_addr == buf->data)
			printf(" (start)");
		if(vm_addr == buf->head)
			printf(" (head)");
		if(vm_addr == buf->tail)
			printf(" (tail)");

		putchar('\n');
	}
}
#endif /* DEBUG */
