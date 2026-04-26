#include "bootlib.h"
#include "munit.h"
#include "snekobject.h"
#include <stdio.h>
#include <stdlib.h>

munit_case(RUN, test_array_set, {
  snek_object_t *foo = new_snek_integer(1);

  snek_object_t *array = new_snek_array(1);
  snek_array_set(array, 0, foo);
  assert_int(foo->refcount, ==, 2, "foo is now referenced by array");

  assert(!boot_is_freed(foo));

  refcount_dec(foo);
  refcount_dec(array);
  assert(boot_all_freed());
});

munit_case(SUBMIT, test_array_free, {
  snek_object_t *foo = new_snek_integer(1);
  snek_object_t *bar = new_snek_integer(2);
  snek_object_t *baz = new_snek_integer(3);

  snek_object_t *array = new_snek_array(2);
  snek_array_set(array, 0, foo);
  snek_array_set(array, 1, bar);
  assert_int(foo->refcount, ==, 2, "foo is now referenced by array");
  assert_int(bar->refcount, ==, 2, "bar is now referenced by array");
  assert_int(baz->refcount, ==, 1, "baz is not yet referenced by array");

  // `foo` is stil referenced in the `array`, so it should not be freed.
  refcount_dec(foo);
  assert(!boot_is_freed(foo));

  // Overwrite index 0, which is `foo`, with `baz`.
  //  Now `foo` is not referenced by `array`, so it should be freed.
  snek_array_set(array, 0, baz);
  assert(boot_is_freed(foo));

  refcount_dec(bar);
  refcount_dec(baz);
  refcount_dec(array);
  assert(boot_all_freed());
});

munit_case(RUN, test_vector3_refcounting, {
  snek_object_t *foo = new_snek_integer(1);
  snek_object_t *bar = new_snek_integer(2);
  snek_object_t *baz = new_snek_integer(3);

  snek_object_t *vec = new_snek_vector3(foo, bar, baz);
  assert_int(foo->refcount, ==, 2, "foo is now referenced by vec");
  assert_int(bar->refcount, ==, 2, "bar is now referenced by vec");
  assert_int(baz->refcount, ==, 2, "baz is now referenced by vec");

  // `foo` is stil referenced in the `vec`, so it should not be freed.
  refcount_dec(foo);
  assert(!boot_is_freed(foo));

  refcount_dec(vec);
  assert(boot_is_freed(foo));

  // These are still alive, they have the original reference still.
  assert(!boot_is_freed(bar));
  assert(!boot_is_freed(baz));

  // Decrement the last reference to the objects, so they will be freed.
  refcount_dec(bar);
  refcount_dec(baz);

  assert(boot_all_freed());
});

munit_case(RUN, test_int_has_refcount, {
  snek_object_t *obj = new_snek_integer(10);
  assert_int(obj->refcount, ==, 1, "Refcount should be 1 on creation");

  free(obj);
});

munit_case(RUN, test_inc_refcount, {
  snek_object_t *obj = new_snek_float(4.20);
  assert_int(obj->refcount, ==, 1, "Refcount should be 1 on creation");

  refcount_inc(obj);
  assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

  free(obj);
});

munit_case(RUN, test_dec_refcount, {
  snek_object_t *obj = new_snek_float(4.20);

  refcount_inc(obj);
  assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

  refcount_dec(obj);
  assert_int(obj->refcount, ==, 1, "Refcount should be decremented");

  assert(!boot_is_freed(obj));

  // Object is still alive, so we will free manually.
  free(obj);
});

munit_case(RUN, test_refcount_free_is_called, {
  snek_object_t *obj = new_snek_float(4.20);

  refcount_inc(obj);
  assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

  refcount_dec(obj);
  assert_int(obj->refcount, ==, 1, "Refcount should be decremented");

  refcount_dec(obj);
  assert(boot_is_freed(obj));
  assert(boot_all_freed());
});

munit_case(RUN, test_allocated_string_is_freed, {
  snek_object_t *obj = new_snek_string("Hello @wagslane!");

  refcount_inc(obj);
  assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

  refcount_dec(obj);
  assert_int(obj->refcount, ==, 1, "Refcount should be decremented");
  assert_string_equal(obj->data.v_string, "Hello @wagslane!", "references str");

  refcount_dec(obj);
  assert(boot_is_freed(obj));
  assert(boot_all_freed());
});

// munit_case(RUN, test_vector3_refcounting, {
//   snek_object_t *foo = new_snek_integer(1);
//   snek_object_t *bar = new_snek_integer(2);
//   snek_object_t *baz = new_snek_integer(3);

//   snek_object_t *vec = new_snek_vector3(foo, bar, baz);
//   assert_int(foo->refcount, ==, 2, "foo is now referenced by vec");
//   assert_int(bar->refcount, ==, 2, "bar is now referenced by vec");
//   assert_int(baz->refcount, ==, 2, "baz is now referenced by vec");

//   // `foo` is stil referenced in the `vec`, so it should not be freed.
//   refcount_dec(foo);
//   assert(!boot_is_freed(foo));

//   refcount_dec(vec);
//   assert(boot_is_freed(foo));

//   // These are still alive, they have the original reference still.
//   assert(!boot_is_freed(bar));
//   assert(!boot_is_freed(baz));

//   // Decrement the last reference to the objects, so they will be freed.
//   refcount_dec(bar);
//   refcount_dec(baz);

//   assert(boot_all_freed());
// });

// munit_case(SUBMIT, test_vector3_refcounting_same, {
//   snek_object_t *foo = new_snek_integer(1);

//   snek_object_t *vec = new_snek_vector3(foo, foo, foo);
//   assert_int(foo->refcount, ==, 4, "foo is now referenced by vec x3");

//   // `foo` is stil referenced in the `vec`, so it should not be freed.
//   refcount_dec(foo);
//   assert(!boot_is_freed(foo));

//   refcount_dec(vec);
//   assert(boot_is_freed(foo));
//   assert(boot_is_freed(vec));
//   assert(boot_all_freed());
// });

// munit_case(RUN, test_int_has_refcount, {
//   snek_object_t *obj = new_snek_integer(10);
//   assert_int(obj->refcount, ==, 1, "Refcount should be 1 on creation");

//   free(obj);
// });

// munit_case(RUN, test_inc_refcount, {
//   snek_object_t *obj = new_snek_float(4.20);
//   assert_int(obj->refcount, ==, 1, "Refcount should be 1 on creation");

//   refcount_inc(obj);
//   assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

//   free(obj);
// });

// munit_case(RUN, test_dec_refcount, {
//   snek_object_t *obj = new_snek_float(4.20);

//   refcount_inc(obj);
//   assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

//   refcount_dec(obj);
//   assert_int(obj->refcount, ==, 1, "Refcount should be decremented");

//   assert(!boot_is_freed(obj));

//   // Object is still alive, so we will free manually.
//   free(obj);
// });

// munit_case(RUN, test_refcount_free_is_called, {
//   snek_object_t *obj = new_snek_float(4.20);

//   refcount_inc(obj);
//   assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

//   refcount_dec(obj);
//   assert_int(obj->refcount, ==, 1, "Refcount should be decremented");

//   refcount_dec(obj);
//   assert(boot_is_freed(obj));
//   assert(boot_all_freed());
// });

// munit_case(RUN, test_allocated_string_is_freed, {
//   snek_object_t *obj = new_snek_string("Hello @wagslane!");

//   refcount_inc(obj);
//   assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

//   refcount_dec(obj);
//   assert_int(obj->refcount, ==, 1, "Refcount should be decremented");
//   assert_string_equal(obj->data.v_string, "Hello @wagslane!", "references str");

//   refcount_dec(obj);
//   assert(boot_is_freed(obj));
//   assert(boot_all_freed());
// });


// munit_case(RUN, test_int_has_refcount, {
//   snek_object_t *obj = new_snek_integer(10);
//   assert_int(obj->refcount, ==, 1, "Refcount should be 1 on creation");

//   free(obj);
// });

// munit_case(RUN, test_inc_refcount, {
//   snek_object_t *obj = new_snek_float(4.20);
//   assert_int(obj->refcount, ==, 1, "Refcount should be 1 on creation");

//   refcount_inc(obj);
//   assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

//   free(obj);
// });

// munit_case(RUN, test_dec_refcount, {
//   snek_object_t *obj = new_snek_float(4.20);

//   refcount_inc(obj);
//   assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

//   refcount_dec(obj);
//   assert_int(obj->refcount, ==, 1, "Refcount should be decremented");

//   assert(!boot_is_freed(obj));

//   // Object is still alive, so we will free manually.
//   free(obj);
// });

// munit_case(SUBMIT, test_refcount_free_is_called, {
//   snek_object_t *obj = new_snek_float(4.20);

//   refcount_inc(obj);
//   assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

//   refcount_dec(obj);
//   assert_int(obj->refcount, ==, 1, "Refcount should be decremented");

//   refcount_dec(obj);
//   assert(boot_is_freed(obj));
//   assert(boot_all_freed());
// });

// munit_case(SUBMIT, test_allocated_string_is_freed, {
//   snek_object_t *obj = new_snek_string("Hello @wagslane!");

//   refcount_inc(obj);
//   assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

//   refcount_dec(obj);
//   assert_int(obj->refcount, ==, 1, "Refcount should be decremented");
//   assert_string_equal(obj->data.v_string, "Hello @wagslane!", "references str");

//   refcount_dec(obj);
//   assert(boot_is_freed(obj));
//   assert(boot_all_freed());
// });

// munit_case(RUN, test_inc_refcount, {
//   snek_object_t *obj = new_snek_integer(10);
//   assert_int(obj->refcount, ==, 1, "Refcount should be 1 on creation");

//   refcount_inc(obj);
//   assert_int(obj->refcount, ==, 2, "Refcount should be incremented");

//   free(obj);
// });

// munit_case(SUBMIT, test_inc_refcount_more, {
//   snek_object_t *obj = new_snek_float(4.20);
//   assert_int(obj->refcount, ==, 1, "Refcount should be 1 on creation");

//   refcount_inc(obj);
//   refcount_inc(obj);
//   refcount_inc(obj);
//   refcount_inc(obj);
//   refcount_inc(obj);
//   assert_int(obj->refcount, ==, 6, "Refcount should be incremented to 6");

//   free(obj);
// });

// munit_case(SUBMIT, test_null_obj, {
//   refcount_inc(NULL);
//   assert(1);
// });


// munit_case(RUN, test_int_has_refcount, {
//   snek_object_t *obj = new_snek_integer(10);
//   assert_int(obj->refcount, ==, 1, "Refcount should be 1 on creation");
//   free(obj);
// });

// munit_case(SUBMIT, test_float_has_refcount, {
//   snek_object_t *obj = new_snek_float(42.0);
//   assert_int(obj->refcount, ==, 1, "Refcount should be 1 on creation");
//   free(obj);
// });

// munit_case(RUN, test_integer_add, {
//   snek_object_t *one = new_snek_integer(1);
//   snek_object_t *three = new_snek_integer(3);
//   snek_object_t *four = snek_add(one, three);

//   assert_not_null(four, "must return an object");
//   assert_int(four->kind, ==, INTEGER, "1 + 3 = 4");
//   assert_int(four->data.v_int, ==, 4, "1 + 3 = 4");

//   free(one);
//   free(three);
//   free(four);
//   assert(boot_all_freed());
// });

munit_case(RUN, test_float_add, {
  snek_object_t *one = new_snek_float(1.5);
  snek_object_t *three = new_snek_float(3.5);
  snek_object_t *five = snek_add(one, three);

  assert_not_null(five, "must return an object");
  assert_int(five->kind, ==, FLOAT, "1.5 + 3.5 = 5.0");
  assert_float(five->data.v_float, ==, 1.5 + 3.5, "1.5 + 3.5 = 5.0");

  free(one);
  free(three);
  free(five);
  assert(boot_all_freed());
});

munit_case(RUN, test_string_add, {
  snek_object_t *hello = new_snek_string("hello");
  snek_object_t *world = new_snek_string(", world");
  snek_object_t *greeting = snek_add(hello, world);

  assert_not_null(greeting, "must return an object");
  assert_int(greeting->kind, ==, STRING, "Must be a string!");
  assert_string_equal(greeting->data.v_string, "hello, world",
                      "Should concatenate strings");

  free(hello->data.v_string);
  free(hello);
  free(world->data.v_string);
  free(world);
  free(greeting->data.v_string);
  free(greeting);
  assert(boot_all_freed());
});

munit_case(SUBMIT, test_string_add_self, {
  snek_object_t *repeated = new_snek_string("(repeated)");
  snek_object_t *result = snek_add(repeated, repeated);

  assert_not_null(result, "must return an object");
  assert_int(result->kind, ==, STRING, "Must be a string!");
  assert_string_equal(result->data.v_string, "(repeated)(repeated)",
                      "Should concatenate strings");

  free(repeated->data.v_string);
  free(repeated);
  free(result->data.v_string);
  free(result);
  assert(boot_all_freed());
});

munit_case(SUBMIT, test_vector3_add, {
  snek_object_t *one = new_snek_float(1.0);
  snek_object_t *two = new_snek_float(2.0);
  snek_object_t *three = new_snek_float(3.0);
  snek_object_t *four = new_snek_float(4.0);
  snek_object_t *five = new_snek_float(5.0);
  snek_object_t *six = new_snek_float(6.0);

  snek_object_t *v1 = new_snek_vector3(one, two, three);
  snek_object_t *v2 = new_snek_vector3(four, five, six);
  snek_object_t *result = snek_add(v1, v2);

  assert_not_null(result, "must return an object");
  assert_int(result->kind, ==, VECTOR3, "Must be a vector3");

  assert_float(result->data.v_vector3.x->data.v_float, ==, 5.0,
               "x component should be 5.0");
  assert_float(result->data.v_vector3.y->data.v_float, ==, 7.0,
               "y component should be 7.0");
  assert_float(result->data.v_vector3.z->data.v_float, ==, 9.0,
               "z component should be 9.0");

  free(v1->data.v_vector3.x);
  free(v1->data.v_vector3.y);
  free(v1->data.v_vector3.z);
  free(v1);

  free(v2->data.v_vector3.x);
  free(v2->data.v_vector3.y);
  free(v2->data.v_vector3.z);
  free(v2);

  free(result->data.v_vector3.x);
  free(result->data.v_vector3.y);
  free(result->data.v_vector3.z);
  free(result);
  assert(boot_all_freed());
});

munit_case(SUBMIT, test_array_add, {
  snek_object_t *one = new_snek_integer(1);
  snek_object_t *ones = new_snek_array(2);
  assert(snek_array_set(ones, 0, one));
  assert(snek_array_set(ones, 1, one));

  snek_object_t *hi = new_snek_string("hi");
  snek_object_t *hellos = new_snek_array(3);
  assert(snek_array_set(hellos, 0, hi));
  assert(snek_array_set(hellos, 1, hi));
  assert(snek_array_set(hellos, 2, hi));

  snek_object_t *result = snek_add(ones, hellos);

  assert_not_null(result, "must return an object");
  assert_int(result->kind, ==, ARRAY, "Must be an array");
  assert_size(result->data.v_array.size, ==, 5, "result should have 5 elements");

  snek_object_t *first = snek_array_get(result, 0);
  assert_not_null(first, "should find the first item");
  assert_int(first->data.v_int, ==, 1, "First item should be an int with 1");

  snek_object_t *second = snek_array_get(result, 1);
  assert_not_null(second, "should find the second item");
  assert_int(second->data.v_int, ==, 1, "Second item should be an int with 1");

  snek_object_t *third = snek_array_get(result, 2);
  assert_not_null(third, "should find the third item");
  assert_string_equal(third->data.v_string, "hi", "third == hi");

  snek_object_t *fourth = snek_array_get(result, 3);
  assert_not_null(fourth, "should find the fourth item");
  assert_string_equal(fourth->data.v_string, "hi", "fourth == hi");

  snek_object_t *fifth = snek_array_get(result, 4);
  assert_not_null(fifth, "should find the fifth item");
  assert_string_equal(fifth->data.v_string, "hi", "fifth == hi");

  free(one);
  free(ones->data.v_array.elements);
  free(ones);

  free(hi->data.v_string);
  free(hi);
  free(hellos->data.v_array.elements);
  free(hellos);
  free(result->data.v_array.elements);
  free(result);

  assert(boot_all_freed());
});

munit_case(SUBMIT, test_array_add_empty_left, {
  snek_object_t *empty = new_snek_array(0);
  snek_object_t *hi = new_snek_string("hi");
  snek_object_t *hellos = new_snek_array(2);
  assert(snek_array_set(hellos, 0, hi));
  assert(snek_array_set(hellos, 1, hi));

  snek_object_t *result = snek_add(empty, hellos);

  assert_not_null(result, "must return an object");
  assert_int(result->kind, ==, ARRAY, "Must be an array");
  assert_size(result->data.v_array.size, ==, 2, "result should have 2 elements");

  snek_object_t *first = snek_array_get(result, 0);
  assert_not_null(first, "should find the first item");
  assert_string_equal(first->data.v_string, "hi", "first == hi");

  snek_object_t *second = snek_array_get(result, 1);
  assert_not_null(second, "should find the second item");
  assert_string_equal(second->data.v_string, "hi", "second == hi");

  free(empty->data.v_array.elements);
  free(empty);
  free(hi->data.v_string);
  free(hi);
  free(hellos->data.v_array.elements);
  free(hellos);
  free(result->data.v_array.elements);
  free(result);

  assert(boot_all_freed());
});

int main() {
  MunitTest tests[] = {
      munit_test("/array_set", test_array_set),
      munit_test("/array_free", test_array_free),
      munit_test("/has_refcount", test_int_has_refcount),
      munit_test("/inc_refcount", test_inc_refcount),
      munit_test("/dec_refcount", test_dec_refcount),
      munit_test("/free_refcount", test_refcount_free_is_called),
      munit_test("/string_freed", test_allocated_string_is_freed),
      munit_test("/vector3", test_vector3_refcounting),
      munit_test("/integer", test_integer_add),
      munit_test("/float", test_float_add),
      munit_test("/string", test_string_add),
      munit_test("/string-repeated", test_string_add_self),
      munit_test("/array", test_array_add),
      munit_test("/array-empty-left", test_array_add_empty_left),
      munit_test("/vector3", test_vector3_add),
      munit_null_test,
  };

  MunitSuite suite = munit_suite("object-add", tests);

  return munit_suite_main(&suite, NULL, 0, NULL);
}
