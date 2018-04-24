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

#include "sync/rwlock.h"

int rwlock_alloc(struct rwlock ** rwlock)
{
	int err;

	*rwlock = malloc(sizeof(**rwlock));
	if(!*rwlock)
		return -ENOMEM;

	err = pthread_mutex_init(&(*rwlock)->lock, NULL);
	if(err)
		goto exit;

	err = pthread_cond_init(&(*rwlock)->cond, NULL);
	if(err)
		goto exit;

	(*rwlock)->readers = 0;
	(*rwlock)->writers = 0;
	(*rwlock)->writing = false;

	return 0;

exit:
	pthread_mutex_destroy(&(*rwlock)->lock);
	pthread_cond_destroy(&(*rwlock)->cond);

	return err;
}

void rwlock_free(struct rwlock ** rwlock)
{
	pthread_mutex_destroy(&(*rwlock)->lock);
	pthread_cond_destroy(&(*rwlock)->cond);

	free(*rwlock);
	*rwlock = NULL;
}

void rwlock_writer_entry(struct rwlock * rwlock)
{
	pthread_mutex_lock(&rwlock->lock);
	rwlock->writers++;
	while(rwlock->readers > 0 || rwlock->writing)
		pthread_cond_wait(&rwlock->cond, &rwlock->lock);

	rwlock->writers--;
	rwlock->writing = true;
	pthread_mutex_unlock(&rwlock->lock);
}

void rwlock_writer_exit(struct rwlock * rwlock)
{
	pthread_mutex_lock(&rwlock->lock);
	rwlock->writing = false;
	pthread_cond_broadcast(&rwlock->cond);
	pthread_mutex_unlock(&rwlock->lock);
}

void rwlock_reader_entry(struct rwlock * rwlock)
{
	pthread_mutex_lock(&rwlock->lock);
	while(rwlock->writing)
		pthread_cond_wait(&rwlock->cond, &rwlock->lock);

	rwlock->readers++;
	pthread_mutex_unlock(&rwlock->lock);
}

void rwlock_reader_exit(struct rwlock * rwlock)
{
	pthread_mutex_lock(&rwlock->lock);
	rwlock->readers--;
	pthread_cond_broadcast(&rwlock->cond);
	pthread_mutex_unlock(&rwlock->lock);
}
