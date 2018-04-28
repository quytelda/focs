/* ring_buffer.c - Ring Buffer Implementation
 * Copyright (C) 2016 Quytelda Kahja
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

static inline void * __rbi_to_vmaddr(const struct ring_buffer * buf,
				     const ssize_t pos)
{
	size_t offset;

	offset = (DS_DATA_SIZE(buf) * pos) % DS_ENTRIES(buf);

	/* Return the calculated address only if it is in-bounds. */
	if(((pos < 0) && (offset >= buf->length)) ||
	   ((pos >= 0) && (offset < buf->length)))
		return (void *) ((size_t) buf->head + offset);

	return NULL;
}

static bool __push_head(struct ring_buffer * buf, const void * data)
{
	void * vm_addr;

	vm_addr = __rbi_to_vmaddr(buf, -1);
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

	vm_addr = __rbi_to_vmaddr(buf, buf->length);
	if(!vm_addr)
		return false;

	buf->tail = __rbi_to_vmaddr(buf, buf->length + 1);
	memcpy(vm_addr, data, DS_DATA_SIZE(buf));

	(buf->length)++;

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
	DS_METADATA_INIT(*buf, props, NULL);

	(*buf)->data = malloc(DS_DATA_SIZE(*buf) * DS_ENTRIES(*buf));
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
