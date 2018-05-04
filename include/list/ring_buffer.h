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

#include <unistd.h>

#include "focs.h"
#include "sync/rwlock.h"

struct ring_buffer {
	DECLARE_DS(NULL);

	void * head;
	void * tail;

	struct rwlock * rwlock;

	size_t length;
	void * data;
};

#define ABS_POS(buf, p) (((p) > 0) ? (p) : ((buf)->length + (p)))

int rb_alloc(struct ring_buffer ** buf,
	     const struct data_properties * props);
void rb_free(struct ring_buffer ** buf);

/* Data Management */
bool rb_push_head(struct ring_buffer * buf, void * data);
bool rb_push_tail(struct ring_buffer * buf, void * data);
void * rb_pop_head(struct ring_buffer * buf);
void * rb_pop_tail(struct ring_buffer * buf);
bool rb_insert(struct ring_buffer * buf, void * data, size_t pos);

#ifdef DEBUG
void rb_show(struct ring_buffer * buf);
#endif /* DEBUG */
