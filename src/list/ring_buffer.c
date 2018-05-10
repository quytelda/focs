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

static inline __pure __nonulls bool __is_index_valid(const ring_buffer buf,
	                                             const size_t absolute)
{
	return (absolute >= 0) && (absolute < __length(buf));
}

static inline __pure __nonulls void * __index_to_addr(const ring_buffer buf,
	                                              const ssize_t relative)
{
	size_t absolute;
	size_t offset;
	size_t start;

	/* Doing arithmetic with void pointers is tricksy, even in GNU C.
	 * Cast all our pointers to size_t integers before doing arithmetic. */
	size_t data = (size_t) DS_PRIV(buf)->data;
	size_t head = (size_t) DS_PRIV(buf)->head;

	absolute = mod(relative, __length(buf)) * DS_DATA_SIZE(buf);
	start = head - data;
	offset = (start + absolute) % __space(buf);
	return (void *) (data + offset);
}

static inline __pure __nonulls size_t __addr_to_index(const ring_buffer buf,
	                                              const void * addr)
{
	/* Doing arithmetic with void pointers is tricksy, even in GNU C.
	 * Cast all our pointers to size_t integers before doing arithmetic. */
	size_t head = (size_t) DS_PRIV(buf)->head;
	size_t mark = (size_t) addr;

	return (mark - head) / DS_DATA_SIZE(buf);
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
		ssize_t pos;

		addr = ((uint8_t *) priv->data) + i;
		pos  = __addr_to_phy(buf, addr);

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
