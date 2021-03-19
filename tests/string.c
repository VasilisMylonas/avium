#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <cmocka.h>

#include <avium/string.h>
#include <avium/object.h>

static int setup(void** state) {
    *state = AvmStringFrom("Hello World Wordy World!");
    return 0;
}

static int teardown(void** state) {
    AvmObjectDestroy(*state);
    return 0;
}

static void avm_string_test_from(void** state) {
    assert_non_null(*state);
    assert_int_equal(AvmStringGetLength(*state), 24);
    assert_int_equal(AvmStringGetCapacity(*state),
                     AVM_STRING_GROWTH_FACTOR * 24);
}

static void avm_string_test_contents(void** state) {
    assert_string_equal(AvmStringAsPtr(*state), "Hello World Wordy World!");
    assert_int_equal(AvmStringCharAt(*state, 2), 'l');
}

static void avm_string_test_indexes(void** state) {
    assert_int_equal(AvmStringIndexOf(*state, 'W'), 6);
    assert_int_equal(AvmStringLastIndexOf(*state, 'W'), 18);
    assert_int_equal(AvmStringFind(*state, "World"), 6);
    assert_int_equal(AvmStringFindLast(*state, "World"), 18);

    assert_int_equal(AvmStringIndexOf(*state, 'x'), AVM_STRING_NPOS);
    assert_int_equal(AvmStringLastIndexOf(*state, 'x'), AVM_STRING_NPOS);
    assert_int_equal(AvmStringFind(*state, "xxl"), AVM_STRING_NPOS);
    assert_int_equal(AvmStringFindLast(*state, "xxl"), AVM_STRING_NPOS);
}

static void avm_string_test_replace(void** state) {
    assert_int_equal(AvmStringReplace(*state, 'o', 'a'), 4);
    assert_string_equal(AvmStringAsPtr(*state), "Hella Warld Wardy Warld!");
}

static void avm_string_test_upper_lower(void** state) {
    AvmStringToUpper(*state);
    assert_string_equal(AvmStringAsPtr(*state), "HELLA WARLD WARDY WARLD!");
    AvmStringToLower(*state);
    assert_string_equal(AvmStringAsPtr(*state), "hella warld wardy warld!");
}

static void avm_string_test_append(void** state) {
    *state = AvmStringAppend(*state, ".com");
    assert_string_equal(AvmStringAsPtr(*state), "hella warld wardy warld!.com");
}

int main(void) {
    cmocka_set_message_output(CM_OUTPUT_XML);

    const struct CMUnitTest tests[] = {
        cmocka_unit_test(avm_string_test_from),
        cmocka_unit_test(avm_string_test_contents),
        cmocka_unit_test(avm_string_test_indexes),
        cmocka_unit_test(avm_string_test_replace),
        cmocka_unit_test(avm_string_test_upper_lower),
        cmocka_unit_test(avm_string_test_append),
    };

    return cmocka_run_group_tests(tests, setup, teardown);
}
