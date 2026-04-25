#include "munit.h"
#include "snekobject.h"
#include <stdlib.h>

munit_case(RUN, test_integer_constant,
           { assert_int(INTEGER, ==, 0, "INTEGER is defined as 0"); });

munit_case(RUN, test_integer_obj, {
  snek_object_t *obj = malloc(sizeof(snek_object_t));
  obj->kind = INTEGER;
  obj->data.v_int = 0;
  assert_int(obj->kind, ==, INTEGER, "must be INTEGER type");
  assert_int(obj->data.v_int, ==, 0, "must equal zero");

  free(obj);
});

int main() {
  MunitTest tests[] = {
      munit_test("/integer_constant", test_integer_constant),
      munit_test("/integer_obj", test_integer_obj),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("object-integer-def", tests);

  return munit_suite_main(&suite, NULL, 0, NULL);
}
