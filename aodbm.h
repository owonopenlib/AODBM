#include <stdbool.h>
#include <stdint.h>

struct aodbm;

struct aodbm_data {
    char *dat;
    size_t sz;
};

typedef struct aodbm aodbm;
typedef struct aodbm_data aodbm_data;

typedef uint64_t aodbm_version;

aodbm *aodbm_open(const char *);
void aodbm_close(aodbm *);

uint64_t aodbm_file_size(aodbm *);

aodbm_version aodbm_current(aodbm *);
bool aodbm_commit(aodbm *, aodbm_version);

bool aodbm_has(aodbm *, aodbm_version, aodbm_data *);
aodbm_version aodbm_set(aodbm *, aodbm_version, aodbm_data *, aodbm_data *);
aodbm_data *aodbm_get(aodbm *, aodbm_version, aodbm_data *);
aodbm_version aodbm_del(aodbm *, aodbm_version, aodbm_data *);
bool aodbm_is_based_on(aodbm *, aodbm_version, aodbm_version);

aodbm_data *aodbm_construct_data(const char *, size_t);
aodbm_data *aodbm_data_dup(aodbm_data *);
aodbm_data *aodbm_data_from_str(const char *);
aodbm_data *aodbm_data_from_32(uint32_t);
aodbm_data *aodbm_data_from_64(uint64_t);
aodbm_data *aodbm_cat_data(aodbm_data *, aodbm_data *);
aodbm_data *aodbm_cat_data_di(aodbm_data *, aodbm_data *);
aodbm_data *aodbm_data_empty();

bool aodbm_data_lt(aodbm_data *, aodbm_data *);
bool aodbm_data_gt(aodbm_data *, aodbm_data *);
bool aodbm_data_lte(aodbm_data *, aodbm_data *);
bool aodbm_data_gte(aodbm_data *, aodbm_data *);
bool aodbm_data_eq(aodbm_data *, aodbm_data *);
int aodbm_data_cmp(aodbm_data *, aodbm_data *);

void aodbm_free_data(aodbm_data *);

void aodbm_print(aodbm_data *);
