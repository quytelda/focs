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
	/* ISO C99 doesn't allow arithmetic on void pointers,
	 * so cast all pointers to size_t integers for arithmetic. */
	const size_t start = (size_t) buf->data;
	const size_t head  = (size_t) buf->head;

	size_t offset;

	offset = mod((ssize_t) DS_DATA_SIZE(buf) * pos, DS_ENTRIES(buf));

	/* Return the calculated address only if it is in-bounds. */
	if(((pos < 0) && (offset >= buf->length)) ||
	   ((pos >= 0) && (offset < buf->length))) {
		size_t roff;
		size_t space;

		roff  = (head - start) + offset;
		space = DS_DATA_SIZE(buf) * DS_ENTRIES(buf);
		return (void *) (start + (roff % space));
	}

	return NULL;
}

static inline bool __is_null(struct ring_buffer * buf)
{
	return (buf->length == 0);
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