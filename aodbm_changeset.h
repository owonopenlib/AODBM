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

#ifndef AODBM_CHANGESET_H
#define AODBM_CHANGESET_H

#define AODBM_MODIFY 1
#define AODBM_REMOVE 2

#include "aodbm_list.h"

struct aodbm_data;

struct aodbm_change {
    unsigned char type;
    struct aodbm_data *key;
    struct aodbm_data *val;
};
typedef struct aodbm_change aodbm_change;

struct aodbm_changeset {
    aodbm_list *list;
};
typedef struct aodbm_changeset aodbm_changeset;

aodbm_changeset aodbm_changeset_empty();
void aodbm_changeset_add_modify
    (aodbm_changeset, struct aodbm_data *, struct aodbm_data *);
void aodbm_changeset_add_remove(aodbm_changeset, struct aodbm_data *);
aodbm_changeset aodbm_changeset_merge_di(aodbm_changeset, aodbm_changeset);
void aodbm_free_changeset(aodbm_changeset);

#endif
