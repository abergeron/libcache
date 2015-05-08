#include "cache.h"

static int null_add(cache *c, cache_key_t k, cache_value_t v) {
  c->kfree(k);
  c->vfree(v);
  return 0;
}

static int null_del(cache *c, const cache_key_t k) {
  return 0;
}

static cache_value_t null_get(cache *c, const cache_key_t k) {
  return NULL;
}

static void null_destroy(cache *c) {}

cache *cache_null(cache_freek_fn kfree, cache_freev_fn vfree) {
  cache *c = malloc(sizeof(*c));
  c->kfree = kfree;
  c->vfree = vfree;
  c->add = null_add;
  c->del = null_del;
  c->get = null_get;
  c->destroy = null_destroy;
  return c;
}
