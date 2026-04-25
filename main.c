#include "munit.h"
#include "snekobject.h"
#include <stdlib.h>

munit_case(RUN, test_positive, {
  snek_object_t *obj = new_snek_float(42);
  assert_float(obj->data.v_float, ==, 42, "Must accept positive values");

  free(obj);
  assert(boot_all_freed());
});

munit_case(SUBMIT, test_zero, {
  snek_object_t *obj = new_snek_float(0.0);

  assert_float(obj->kind, ==, FLOAT, "Must set type to FLOAT");
  assert_float(obj->data.v_float, ==, 0.0, "Must accept 0.0");

  free(obj);
  assert(boot_all_freed());
});

munit_case(SUBMIT, test_negative, {
  snek_object_t *obj = new_snek_float(-5.0);

  assert_float(obj->kind, ==, FLOAT, "Must set type to FLOAT");
  assert_float(obj->data.v_float, ==, -5.0, "Must accept negative numbers");

  free(obj);
  assert(boot_all_freed());
});

int main() {
  MunitTest tests[] = {
      munit_test("/positive", test_positive),
      munit_test("/zero", test_zero),
      munit_test("/negative", test_negative),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("object-float", tests);

  return munit_suite_main(&suite, NULL, 0, NULL);
}
