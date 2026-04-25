#include "munit.h"
#include "snekobject.h"
#include <stdlib.h>

munit_case(RUN, test_array, {
  // Create an array
  snek_object_t *obj = new_snek_array(2);

  // Create some objects to put in the array
  // Can store objects, even though they have different types (just like Python)
  snek_object_t *first = new_snek_string("First");
  snek_object_t *second = new_snek_integer(3);

  assert(snek_array_set(obj, 0, first));
  assert(snek_array_set(obj, 1, second));

  snek_object_t *retrieved_first = snek_array_get(obj, 0);
  assert_not_null(retrieved_first, "Should find the first object");
  assert_int(retrieved_first->kind, ==, STRING, "Should be a string");
  assert_ptr(first, ==, retrieved_first, "Should be the same object");

  snek_object_t *retrieved_second = snek_array_get(obj, 1);
  assert_not_null(retrieved_second, "Should find the second object");
  assert_int(retrieved_second->kind, ==, INTEGER, "Should be a integer");
  assert_ptr(second, ==, retrieved_second, "Should be the same object");

  free(first->data.v_string);
  free(first);
  free(second);
  free(obj->data.v_array.elements);
  free(obj);
  assert(boot_all_freed());
});

munit_case(RUN, test_set_outside_bounds, {
  // Create an array
  snek_object_t *obj = new_snek_array(2);

  snek_object_t *outside = new_snek_string("First");

  // Inside of bound
  assert(snek_array_set(obj, 1, outside));

  // Outside of bound
  assert_false(snek_array_set(obj, 100, outside));

  // Free memory
  free(outside->data.v_string);
  free(outside);
  free(obj->data.v_array.elements);
  free(obj);
  assert(boot_all_freed());
});

munit_case(SUBMIT, test_get_outside_bounds, {
  // Create an array
  snek_object_t *obj = new_snek_array(1);
  snek_object_t *first = new_snek_string("First");
  assert(snek_array_set(obj, 0, first));

  // Outside of bound
  assert_null(snek_array_get(obj, 1), "Should not access outside the array");

  free(first->data.v_string);
  free(first);
  free(obj->data.v_array.elements);
  free(obj);
  assert(boot_all_freed());
});

int main() {
  MunitTest tests[] = {
      munit_test("/set_and_get", test_array),
      munit_test("/set_outside", test_set_outside_bounds),
      munit_test("/get_outside", test_get_outside_bounds),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("object-array", tests);

  return munit_suite_main(&suite, NULL, 0, NULL);
}
