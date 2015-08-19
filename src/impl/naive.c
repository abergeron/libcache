#include "cache.h"

struct pair {
  cache_key_t k;
  cache_value_t v;
};

typedef struct _cache_naive {
  cache c;
  size_t size;
  struct pair kv[];
} cn;

void nofree(void *);

static int naive_add(cache *c, cache_key_t k, cache_value_t v);
static int naive_del(cache *c, cache_key_t k);
static cache_value_t naive_get(cache *c, cache_key_t k);
static void naive_destroy(cache *c);

cache *cache_naive(size_t size, cache_eq_fn keq, cache_freek_fn kfree,
                   cache_freev_fn vfree) {
  cache *c = calloc(sizeof(struct pair),
                    (sizeof(struct _cache)/sizeof(struct pair)) + size + 1);
  c->keq = keq;
  c->kfree = kfree == NULL ? nofree : kfree;
  c->vfree = vfree == NULL ? nofree : vfree;
  c->add = naive_add;
  c->del = naive_del;
  c->get = naive_get;
  c->destroy = naive_destroy;
  ((cn *)c)->size = size;
  return c;
}

int naive_add(cache *_c, cache_key_t k, cache_value_t v) {
  size_t i;
  cn *c = (cn *)_c;
  for (i = 0; i < c->size; i++) {
    if (c->kv[i].k == NULL || c->c.keq(k, c->kv[i].k)) {
      if (c->kv[i].k != NULL) {
        c->c.kfree(c->kv[i].k);
        c->c.vfree(c->kv[i].v);
      }
      c->kv[i].k = k;
      c->kv[i].v = v;
      return 0;
    }
  }
  c->c.kfree(k);
  c->c.vfree(v);
  return 1;
}

int naive_del(cache *_c, cache_key_t k) {
  size_t i;
  cn *c = (cn *)_c;
  for (i = 0; i < c->size; i++) {
    if (c->kv[i].k != NULL) {
      if (c->c.keq(k, c->kv[i].k)) {
        c->c.kfree(c->kv[i].k);
        c->c.vfree(c->kv[i].v);
        c->kv[i].k = NULL;
        return 1;
      }
    }
  }
  return 0;
}

cache_value_t naive_get(cache *_c, cache_key_t k) {
  size_t i;
  cn *c = (cn *)_c;
  for (i = 0; i < c->size; i++) {
    if (c->kv[i].k != NULL) {
      if (c->c.keq(k, c->kv[i].k)) {
        return c->kv[i].v;
      }
    }
  }
  return NULL;
}

void naive_destroy(cache *_c) {
  size_t i;
  cn *c = (cn *)_c;
  for (i = 0; i < c->size; i++) {
    if (c->kv[i].k != NULL) {
      c->c.kfree(c->kv[i].k);
      c->c.vfree(c->kv[i].v);
    }
  }
}
