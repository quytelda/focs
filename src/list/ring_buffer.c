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

static bool __push_head(struct ring_buffer * buf, const void * data)
{
	void * vm_addr;

	vm_addr = __rbpos_to_addr(buf, -1);
	if(!vm_addr)
		return false;

	buf->head = vm_addr;
	memcpy(vm_addr, data, DS_DATA_SIZE(buf));

	(buf->length)++;

	return true;
}

static bool __push_tail(struct ring_buffer * buf, const void * data)
{
	void * vm_addr;

	vm_addr = __rbpos_to_addr(buf, buf->length);
	if(!vm_addr)
		return false;

	buf->tail = __rbpos_to_addr(buf, buf->length + 1);
	memcpy(vm_addr, data, DS_DATA_SIZE(buf));

	(buf->length)++;

	return true;
}

static void * __pop_head(struct ring_buffer * buf)
{
	void * addr;
	void * data;

	if(__is_null(buf))
		return NULL;

	addr = __rbpos_to_addr(buf, 0);
	if(!addr)
		return NULL;

	data = malloc(DS_DATA_SIZE(buf));
	if(!data)
		return NULL;

	buf->head = __rbpos_to_addr(buf, 1);
	memcpy(data, addr, DS_DATA_SIZE(buf));

	(buf->length)--;

	return data;
}

static void * __pop_tail(struct ring_buffer * buf)
{
	void * addr;
	void * data;

	if(__is_null(buf))
		return NULL;

	addr = __rbpos_to_addr(buf, buf->length - 1);
	if(!addr)
		return NULL;

	data = malloc(DS_DATA_SIZE(buf));
	if(!data)
		return NULL;

	buf->tail = __rbpos_to_addr(buf, buf->length - 1);
	memcpy(data, addr, DS_DATA_SIZE(buf));

	(buf->length)--;

	return data;
}

static bool __insert(struct ring_buffer * buf, void * data, ssize_t pos)
{
	void * addr;
	uint8_t * src;
	uint8_t * dest;

	if(__is_full(buf))
		return false;

	if(pos >= DS_ENTRIES(buf))
		return false;

	addr = __rbpos_to_addr(buf, pos);
	if(!addr)
		return false;

	if(ABS_POS(buf, pos) > buf->length) {
		buf->length = pos + 1;
	} else {
		(buf->length)++;

		dest = __rbpos_to_addr(buf, buf->length);
		for(ssize_t i = buf->length - 1; i >= pos; i--) {
			src = __rbpos_to_addr(buf, i);
			memcpy(dest, src, DS_DATA_SIZE(buf));
			dest = src;
		}
	}

	memcpy(addr, data, DS_DATA_SIZE(buf));
	buf->tail = __rbpos_to_addr(buf, buf->length);

	return true;
}

int rb_alloc(struct ring_buffer ** buf,
		   const struct data_properties * props)
{
	*buf = malloc(sizeof(**buf));
	if(!*buf) {
		errno = ENOMEM;
		goto exit;
	}
	DS_SET_PROPS(*buf, props);

	(*buf)->data = calloc(DS_ENTRIES(*buf), DS_DATA_SIZE(*buf));
	if(!(*buf)->data) {
		errno = ENOMEM;
		goto exit;
	}

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

bool rb_push_head(struct ring_buffer * buf, void * data)
{
	bool success;

	rwlock_writer_entry(buf->rwlock);
	success = __push_head(buf, data);
	rwlock_writer_exit(buf->rwlock);

	return success;
}

bool rb_push_tail(struct ring_buffer * buf, void * data)
{
	bool success;

	rwlock_writer_entry(buf->rwlock);
	success = __push_tail(buf, data);
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

bool rb_insert(struct ring_buffer * buf, void * data, size_t pos)
{
	bool success;

	rwlock_writer_entry(buf->rwlock);
	success = __insert(buf, data, pos);
	rwlock_writer_exit(buf->rwlock);

	return success;
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

		printf("%p (%ld): %#04x", vm_addr, ABS_POS(pos), *vm_addr);

		if(vm_addr == buf->data)
			printf(" (start)");
		if(vm_addr == buf->end)
			printf(" (end)");
		if(vm_addr == buf->head)
			printf(" (head)");
		if(vm_addr == buf->tail)
			printf(" (tail)");

		putchar('\n');
	}
}
#endif /* DEBUG */
