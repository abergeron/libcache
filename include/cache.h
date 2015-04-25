#ifndef CACHE_H
#define CACHE_H

#ifndef CACHE_BUILD_STATIC
# ifdef _WIN32
#  ifdef CACHE_BUILD_DLL
#   define CACHE_API __declspec(dllexport)
#  else
#   define CACHE_API __declspec(dllimport)
#  endif
# else
#  if defined(__clang__) || (defined(__GNUC_) && __GNUC__ >= 4)
#   define CACHE_API __attribute__((visibility ("default")))
#  else
#   define CACHE_API /* This is a last-resort fallback */
#  endif
#else
# define CACHE_API /* Static build has no symbols to export */
#endif

typedef void *cache_key_t;
typedef void *cache_value_t;

typedef int (*cache_eq_fn)(cache_key_t, cache_key_t);
typedef uint32_t (*cache_hash_fn)(cache_key_t);
typedef void (*cache_freek_fn)(cache_key_t);
typedef void (*cache_freev_fn)(cache_value_t);

typedef struct _cache {
  int (*add)(cache *c, cache_key_t k, cache_value_t v);
  int (*del)(cache *c, cache_key_t k);
  cache_value_t (*get)(cache *c, cache_key_t k);
  void (*destroy)(cache *c);
  cache_eq_fn keq;
  cache_hash_fn khash;
  cache_free_fn kfree;
  cache_free_fn vfree;
  /* Extra data goes here depending on cache type */
} cache;


/* Cache creators */
CACHE_API cache *cache_naive(size_t size, cache_eq_fn keq,
                             cache_freek_fn kfree, cache_freev_fn vfree);


/* API functions */
static inline int cache_add(cache *c, cache_key_t k, cache_value_t v) {
  return c->add(c, k, v);
}

static inline int cache_del(cache *c, cache_key_t k) {
  return c->del(c, k);
}

static inline cache_value_t cache_get(cache *c, cache_key_t k) {
  return c->get(c, k);
}

static inline void cache_destroy(cache *c) {
  c->destroy(c);
  free(c);
}

#endif
