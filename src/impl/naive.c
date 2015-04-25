#include "cache.h"

struct pair {
  cache_key_t k;
  cache_value_t v;
};

struct cache_naive {
  cache c;
  size_t size;
  struct pair kv[];
};

static naive_add(cache *c, cache_key_ k, cache_value_t v);
static naive_del(cache *c, cache_key_ k);
static naive_get(cache *c, cache_key_ k);
static naive_destroy(cache *c);

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
  ((cache_naive *)c)->size = size;
}

int naive_add(cache *_c, cache_key_t k, cache_value_t v) {
  cache_naive *c = (cache_naive *)_c;
  for (size_t i = 0; i < c->size; i++) {
    if (c->kv[i].k == NULL) {
      c->kv[i].k = k;
      c->kv[i].v = v;
      return 1;
    }
  }
  return 0;
}

int naive_del(cache *_c, cache_key_t k) {
  cache_naive *c = (cache_naive *)_c;
  for (size_t i = 0; i < c->size; i++) {
    if (c->kv[i].k != NULL) {
      if (c->keq(k, c->kv[i].k)) {
        c->kfree(c->kv[i].k);
        c->vfree(c->kv[i].v);
        c->kv[i].k = NULL;
        return 1;
      }
    }
  }
  return 0;
}

cache_value_t naive_get(cache *_c, cache_key_t k) {
  cache_naive *c = (cache_naive *)_c;
  for (size_t i = 0; i < c->size; i++) {
    if (c->kv[i].k != NULL) {
      if (c->keq(k, c->kv[i].k)) {
        return c->kv[i].v;
      }
    }
  }
  return NULL;
}

void naive_destroy(cache *_c) {
  cache_naive *c = (cache_naive *)_c;
  for (size_t i = 0; i < c->size; i++) {
    if (c->kv[i].k != NULL) {
      c->kfree(c->kv[i].k);
      c->vfree(c->kv[i].v);
    }
  }
}
