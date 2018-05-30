/* rwlock.c - Reader/Writer Lock Implementation
 * Copyright (C) 2018 Quytelda Kahja
 *
 * This file is part of focs.
 *
 * focs is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#ifndef __RWLOCK_H
#define __RWLOCK_H

#include <errno.h>
#include <pthread.h>

#include "focs.h"

struct rwlock {
	pthread_mutex_t lock;
	pthread_cond_t cond;
	size_t readers;
	size_t writers;
	bool writing;
};

int rwlock_create(struct rwlock ** rwlock);
void rwlock_free(struct rwlock ** rwlock);
void rwlock_writer_entry(struct rwlock * rwlock);
void rwlock_writer_exit(struct rwlock * rwlock);
void rwlock_reader_entry(struct rwlock * rwlock);
void rwlock_reader_exit(struct rwlock * rwlock);

#endif /* __RWLOCK_H */
