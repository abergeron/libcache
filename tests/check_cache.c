#include <check.h>

#include "cache.h"

static char *k0 = "0000000";
static char *k1 = "aaaaaaa";
static char *k2 = "aaaaaab";

static char *v0 = "0";
static char *v1 = "1";
static char *v2 = "2";
static char *v3 = "3";

static int str_eq(void *k1, void *k2) {
  return strcmp((char *)k1, (char *)k2) == 0;
}

static uint32_t str_hash(void *k1) {
  uint32_t hash = 5381;
  char *p = k1;
  int c;

  while ((c = *p++))
    hash = (hash * 33) ^ c;

  return hash;
}

/* Validate basic cache api is correct */
static void ctest_validate(cache *c) {
  /* Check that add goes ok */
  if (cache_add(c, k0, v0) == 0)
    ck_assert_ptr_eq(v0, cache_get(c, k0));
  if (cache_add(c, k1, v1) == 0)
    ck_assert_ptr_eq(v1, cache_get(c, k1));
  if (cache_add(c, k2, v2) == 0)
    ck_assert_ptr_eq(v2, cache_get(c, k2));

  /* Try to replace one of the keys */
  if (cache_get(c, k0) != NULL && cache_add(c, k0, v3) == 0) {
    ck_assert_ptr_eq(v3, cache_get(c, k0));
    goto next;
  }

  if (cache_get(c, k1) != NULL && cache_add(c, k1, v3) == 0) {
    ck_assert_ptr_eq(v3, cache_get(c, k1));
    goto next;
  }

  if (cache_get(c, k2) != NULL && cache_add(c, k2, v3) == 0) {
    ck_assert_ptr_eq(v3, cache_get(c, k2));
    goto next;
  }

 next:
  /* Check that non-exsitent keys return NULL */
  ck_assert_ptr_eq(NULL, cache_get(c, ""));
  ck_assert_ptr_eq(NULL, cache_get(c, "potato"));

  /* Check that del returns the right value and the key isn't found anymore */
  if (cache_get(c, k0) != NULL)
    ck_assert_int_eq(1, cache_del(c, k0));
  else
    ck_assert_int_eq(0, cache_del(c, k0));
  ck_assert_ptr_eq(NULL, cache_get(c, k0));

  if (cache_get(c, k1) != NULL)
    ck_assert_int_eq(1, cache_del(c, k1));
  else
    ck_assert_int_eq(0, cache_del(c, k1));
  ck_assert_ptr_eq(NULL, cache_get(c, k1));

  if (cache_get(c, k2) != NULL)
    ck_assert_int_eq(1, cache_del(c, k2));
  else
    ck_assert_int_eq(0, cache_del(c, k2));
  ck_assert_ptr_eq(NULL, cache_get(c, k2));

  cache_destroy(c);
}

struct k {
  int k;
  int a;
};

struct v {
  int a;
};

static int k_eq(void *k1, void *k2) {
  return ((struct k *)k1)->k == ((struct k *)k2)->k;
}

static uint32_t k_hash(void *k) {
  return ((struct k *)k)->k;
}

static void k_free(void *k) {
  ((struct k *)k)->a = 0;
}

static void v_free(void *v) {
  ((struct v *)v)->a = 0;
}

/* Validate cache frees data correctly */
static void ctest_freeok(cache *c) {
  struct k k1 = {0, 1};
  struct k k2 = {1, 1};
  struct k k3 = {0, 1};
  struct v v1 = {1};
  struct v v2 = {1};
  struct v v3 = {1};

  ck_assert_int_eq(0, cache_add(c, &k1, &v1));
  ck_assert_int_eq(1, k1.a);
  ck_assert_int_eq(1, v1.a);
  
  ck_assert_int_eq(0, cache_add(c, &k2, &v2));
  ck_assert_int_eq(1, k2.a);
  ck_assert_int_eq(1, v2.a);

  cache_get(c, &k1);
  cache_get(c, &k2);

  ck_assert_int_eq(1, k1.a);
  ck_assert_int_eq(1, v1.a);
  ck_assert_int_eq(1, k2.a);
  ck_assert_int_eq(1, v2.a);

  ck_assert_ptr_eq(&v1, cache_get(c, &k3));

  ck_assert_int_eq(1, k3.a);

  ck_assert_int_eq(0, cache_add(c, &k3, &v3));
  ck_assert_int_eq(0, k1.a);
  ck_assert_int_eq(0, v1.a);
  ck_assert_int_eq(1, k3.a);
  ck_assert_int_eq(1, v3.a);

  ck_assert_int_eq(1, cache_del(c, &k1));
  ck_assert_int_eq(0, k3.a);
  ck_assert_int_eq(0, v3.a);

  ck_assert_int_eq(1, k2.a);
  ck_assert_int_eq(1, v2.a);

  cache_destroy(c);
}

START_TEST(test_cache_naive)
{

  cache *c = cache_naive(4, str_eq, nofree, nofree);
  ck_assert_ptr_ne(NULL, c);
  ctest_validate(c);

  c = cache_naive(2, k_eq, k_free, v_free);
  ck_assert_ptr_ne(NULL, c);  
  ctest_freeok(c);
}
END_TEST

START_TEST(test_cache_lru)
{

  cache *c = cache_lru(4, 2, str_eq, str_hash, nofree, nofree);
  ck_assert_ptr_ne(NULL, c);
  ctest_validate(c);

  c = cache_lru(2, 1, k_eq, k_hash, k_free, v_free);
  ck_assert_ptr_ne(NULL, c);
  ctest_freeok(c);
}
END_TEST

START_TEST(test_cache_twoq)
{

  cache *c = cache_twoq(1, 2, 1, 2, str_eq, str_hash, nofree, nofree);
  ck_assert_ptr_ne(NULL, c);
  ctest_validate(c);

  c = cache_twoq(1, 2, 1, 1, k_eq, k_hash, k_free, v_free);
  ck_assert_ptr_ne(NULL, c);
  ctest_freeok(c);
}
END_TEST

Suite *get_suite(void) {
  Suite *s = suite_create("cache");
  TCase *tc = tcase_create("All");
  tcase_add_test(tc, test_cache_naive);
  tcase_add_test(tc, test_cache_lru);
  tcase_add_test(tc, test_cache_twoq);
  suite_add_tcase(s, tc);
  return s;
}
