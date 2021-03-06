/*  
    Copyright (C) 2011 aodbm authors,
    
    This file is part of aodbm.
    
    aodbm is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    aodbm is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"

#include "aodbm_rwlock.h"
#include "aodbm_error.h"

void aodbm_rwlock_init(aodbm_rwlock_t *lock) {
    pthread_mutex_init(&lock->mut, NULL);
    pthread_cond_init(&lock->cnd, NULL);
    lock->readers = 0;
    lock->is_writing = false;
    lock->writers_waiting = 0;
}

void aodbm_rwlock_destroy(aodbm_rwlock_t *lock) {
    pthread_mutex_destroy(&lock->mut);
    pthread_cond_destroy(&lock->cnd);
}

void aodbm_rwlock_rdlock(aodbm_rwlock_t *lock) {
    pthread_mutex_lock(&lock->mut);
    while (lock->is_writing || lock->writers_waiting != 0) {
        pthread_cond_wait(&lock->cnd, &lock->mut);
    }
    lock->readers += 1;
    pthread_mutex_unlock(&lock->mut);
}

void aodbm_rwlock_wrlock(aodbm_rwlock_t *lock) {
    pthread_mutex_lock(&lock->mut);
    lock->writers_waiting += 1;
    while (lock->readers != 0 || lock->is_writing) {
        pthread_cond_wait(&lock->cnd, &lock->mut);
    }
    lock->is_writing = true;
    lock->writers_waiting -= 1;
    pthread_mutex_unlock(&lock->mut);
}

void aodbm_rwlock_unlock(aodbm_rwlock_t *lock) {
    pthread_mutex_lock(&lock->mut);
    if (lock->is_writing) {
        lock->is_writing = false;
        pthread_cond_broadcast(&lock->cnd);
    } else if (lock->readers > 0) {
        lock->readers -= 1;
        if (lock->readers == 0) {
            pthread_cond_broadcast(&lock->cnd);
        }
    } else {
        AODBM_CUSTOM_ERROR("invalid rwlock state");
    }
    pthread_mutex_unlock(&lock->mut);
}

bool aodbm_rwlock_tryrdlock(aodbm_rwlock_t *lock) {
    bool result;
    pthread_mutex_lock(&lock->mut);
    result = !lock->is_writing && lock->writers_waiting == 0;
    if (result) {
        lock->readers += 1;
    }
    pthread_mutex_unlock(&lock->mut);
    return result;
}

bool aodbm_rwlock_trywrlock(aodbm_rwlock_t *lock) {
    bool result;
    pthread_mutex_lock(&lock->mut);
    result = lock->readers == 0 && !lock->is_writing;
    if (result) {
        lock->is_writing = true;
    }
    pthread_mutex_unlock(&lock->mut);
    return result;
}
