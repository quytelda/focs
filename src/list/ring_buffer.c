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

int rb_alloc(struct ring_buffer ** buf,
		   const struct data_properties * props)
{
	int err;

	*buf = malloc(sizeof(**buf));
	if(!*buf) {
		err = -ENOMEM;
		goto exit;
	}

	DS_METADATA_INIT(*buf, props, NULL);
	err = rwlock_alloc(&(*buf)->rwlock);
	if(err)
		goto exit;

	return 0;

exit:
	free(*buf);
	return err;

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
