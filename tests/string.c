#include <avium/testing.h>
#include <avium/runtime.h>
#include <avium/string.h>

void TestNew(size_t capacity) {
    const size_t length = 0;

    AvmString s = AvmStringNew(capacity);

    if (capacity == 0) {
        AssertNull(AvmStringGetBuffer(&s));
    } else {
        AssertNotNull(AvmStringGetBuffer(&s));
    }

    AssertEq(AvmStringGetLength(&s), length);
    AssertEq(AvmStringGetCapacity(&s), capacity);
    AvmObjectDestroy(&s);
}

void TestFrom(str string) {
    const size_t length = strlen(string);

    AvmString s = AvmStringFrom(string);

    if (length == 0) {
        AssertNull(AvmStringGetBuffer(&s));
    } else {
        AssertNotNull(AvmStringGetBuffer(&s));
        AssertEq(AvmStringGetBuffer(&s), string);
    }

    AssertEq(AvmStringGetLength(&s), length);
    AssertEq(AvmStringGetCapacity(&s), length * AVM_STRING_GROWTH_FACTOR);
    AvmObjectDestroy(&s);
}

void TestFromChars(size_t length, char array[]) {
    AvmString s = AvmStringFromChars(length, array);

    if (length == 0) {
        AssertNull(AvmStringGetBuffer(&s));
    } else {
        AssertNotNull(AvmStringGetBuffer(&s));
        AssertMemEq(AvmStringGetBuffer(&s), array, length);
    }

    AssertEq(AvmStringGetLength(&s), length);
    AssertEq(AvmStringGetCapacity(&s), length * AVM_STRING_GROWTH_FACTOR);
    AvmObjectDestroy(&s);
}

void TestRepeat(str string, size_t count) {
    const size_t length = strlen(string);

    AvmString s = AvmStringRepeat(string, count);

    AssertEq(AvmStringGetLength(&s), count * length);
    AssertEq(AvmStringGetCapacity(&s),
             count * length * AVM_STRING_GROWTH_FACTOR);

    if (length == 0 || count == 0) {
        AssertNull(AvmStringGetBuffer(&s));
    } else {
        AssertNotNull(AvmStringGetBuffer(&s));
        AssertMemEq(AvmStringGetBuffer(&s) + 0, string, length);
        AssertMemEq(AvmStringGetBuffer(&s) + (count - 1) * length, string,
                    length);
    }

    AvmObjectDestroy(&s);
}

void TestPush(str string) {
    AvmString s = AvmStringFrom(string);
    AvmStringPushStr(&s, "!!");
    AssertMemEq(AvmStringGetBuffer(&s), string, strlen(string));
    AssertMemEq(AvmStringGetBuffer(&s) + strlen(string), "!!", 2);
}

// void TestRepeatChars(size_t length, char array[], size_t count) {
//     AvmString s = AvmStringRepeatChars(length, array, count);

//     if (length == 0 || count == 0) {
//         AssertNull(AvmStringGetBuffer(&s));
//     } else {
//         AssertNotNull(AvmStringGetBuffer(&s));
//     }

//     AssertEq(AvmStringGetLength(&s), length * count);
//     AssertEq(AvmStringGetCapacity(&s),
//              length * count * AVM_STRING_GROWTH_FACTOR);

//     AssertMemEq(AvmStringGetBuffer(&s) + 0, array, length);
//     AssertMemEq(AvmStringGetBuffer(&s) + count, array, length);

//     AvmObjectDestroy(&s);
// }

// void TestReplaceAll(AvmString* state) {
//     AssertEq(AvmStringReplaceAll(state, 'o', 'a'), 2);
//     AssertEq(AvmStringGetBuffer(state), "Hella Warld!");
// }

// void TestUpperLower(AvmString* state) {
//     AvmStringToUpper(state);
//     AssertEq(AvmStringGetBuffer(state), "HELLO WORLD!");
//     AvmStringToLower(state);
//     AssertEq(AvmStringGetBuffer(state), "hello world!");
// }

// void TestReplaceN(AvmString* state) {
//     AssertEq(AvmStringReplaceN(state, 1, 'o', 'a'), 1);
//     AssertEq(AvmStringGetBuffer(state), "Hella World!");
// }

// void TestReplaceLastN(AvmString* state) {
//     AssertEq(AvmStringReplaceLastN(state, 1, 'o', 'a'), 1);
//     AssertEq(AvmStringGetBuffer(state), "Hello Warld!");
// }

// void TestReplace(AvmString* state) {
//     AssertEq(AvmStringReplace(state, 'o', 'a'), 4);
//     AssertEq(AvmStringGetBuffer(state), "Hella World!");

//     AssertEq(AvmStringReplace(state, 'u', 'a'), AvmInvalid);
//     AssertEq(AvmStringGetBuffer(state), "Hella World!");
// }

// void TestReplaceLast(AvmString* state) {
//     AssertEq(AvmStringReplaceLast(state, 'o', 'a'), 7);
//     AssertEq(AvmStringGetBuffer(state), "Hello Warld!");

//     AssertEq(AvmStringReplaceLast(state, 'u', 'a'), AvmInvalid);
//     AssertEq(AvmStringGetBuffer(state), "Hello Warld!");
// }

// void TestStartsWithStr(AvmString* state) {
//     AssertTrue(AvmStringStartsWithStr(state, "Hello"));
//     AssertFalse(AvmStringStartsWithStr(state, "Hella"));
// }

// void TestStartsWithChar(AvmString* state) {
//     AssertTrue(AvmStringStartsWithChar(state, 'H'));
//     AssertFalse(AvmStringStartsWithChar(state, 'e'));
// }

// void TestEndsWithStr(AvmString* state) {
//     AssertTrue(AvmStringEndsWithStr(state, "World!"));
//     AssertFalse(AvmStringEndsWithStr(state, "Hello"));
// }

// void TestEndsWithChar(AvmString* state) {
//     AssertTrue(AvmStringEndsWithChar(state, '!'));
//     AssertFalse(AvmStringEndsWithChar(state, 'H'));
// }

// void TestContainsStr(AvmString* state) {
//     AssertTrue(AvmStringContainsStr(state, "ello W"));
//     AssertFalse(AvmStringContainsStr(state, "ello X"));
// }

// void TestContainsChar(AvmString* state) {
//     AssertTrue(AvmStringContainsChar(state, 'e'));
//     AssertFalse(AvmStringContainsChar(state, 'X'));
// }

// void TestReverse(AvmString* state) {
//     AvmStringReverse(state);
//     AssertMemEq(AvmStringGetBuffer(state), "!dlroW olleH",
//                 AvmStringGetLength(state));
// }

// void TestClear(AvmString* state) {
//     AvmStringClear(state);
//     AssertEq(AvmStringGetLength(state), 0);
// }

// void TestErase(AvmString* state) {
//     byte desired[12] = {0};
//     AvmStringErase(state);
//     AssertMemEq(AvmStringGetBuffer(state), desired, sizeof(desired));
//     AssertEq(AvmStringGetLength(state), 0);
// }

// void TestUnsafeSetLength(AvmString* state) {
//     AvmStringUnsafeSetLength(state, 5360);
//     AssertEq(AvmStringGetLength(state), 5360);
// }

// void TestCharAt(AvmString* state) {
//     AssertEq(AvmStringCharAt(state, 0, NULL), 'H');
//     AssertEq(AvmStringCharAt(state, 100, NULL), '\0');
// }

// void TestIndexes() {
//     AvmString s = AvmStringFrom("Hello Worldy World!");

//     AssertEq(AvmStringIndexOf(&s, 'W'), 6);
//     AssertEq(AvmStringLastIndexOf(&s, 'W'), 13);
//     AssertEq(AvmStringFind(&s, "World"), 6);
//     AssertEq(AvmStringFindLast(&s, "World"), 13);
//     AssertEq(AvmStringIndexOf(&s, 'x'), AvmInvalid);
//     AssertEq(AvmStringLastIndexOf(&s, 'x'), AvmInvalid);
//     AssertEq(AvmStringFind(&s, "xxl"), AvmInvalid);
//     AssertEq(AvmStringFindLast(&s, "xxl"), AvmInvalid);
// }

// void TestToString(AvmString* state) {
//     AvmString s = AvmObjectToString(state);
//     AssertEq(AvmStringGetLength(state), AvmStringGetLength(&s));
//     AssertMemEq(AvmStringGetBuffer(state), AvmStringGetBuffer(&s),
//                 AvmStringGetLength(state));
// }

// void TestClone(AvmString* state) {
//     AvmString* s = AvmObjectClone(state);
//     AssertEq(AvmStringGetLength(state), AvmStringGetLength(s));
//     AssertEq(AvmStringGetLength(state), AvmStringGetLength(s));
//     AssertMemEq(AvmStringGetBuffer(state), AvmStringGetBuffer(s),
//                 AvmStringGetLength(state));
// }

// void TestIsEmpty(AvmString* state) {
//     AssertFalse(AvmStringIsEmpty(state));

//     AvmString s = AvmStringNew(0);
//     AssertTrue(AvmStringIsEmpty(&s));
// }

// void TestUnsafeDestruct(AvmString* state) {
//     size_t capacity;
//     size_t length;
//     char* buffer;

//     AvmStringUnsafeDestruct(state, &capacity, &length, &buffer);

//     AssertEq(capacity, AvmStringGetCapacity(state));
//     AssertEq(length, AvmStringGetLength(state));
//     AssertEq(buffer, AvmStringGetBuffer(state));
// }

test {
    AvmEnableExceptions();

    for (size_t i = 0; i < 32; i++) {
        TestNew(i);
    }

    TestFrom("");
    TestFrom("Hello");

    TestFromChars(3, "");
    TestFromChars(0, "");
    TestFromChars(0, "Hello");
    TestFromChars(5, "Hello");
    TestFromChars(3, "Hello");

    TestRepeat("", 0);
    TestRepeat("", 5);
    TestRepeat("Hello", 5);
    TestRepeat("Hello", 3);

    TestPush("Hello World");

    // TestRepeatChars(0, "", 5);
    // TestRepeatChars(0, "", 0);
    // TestRepeatChars(5, "Hello", 0);
    // TestRepeatChars(5, "Hello", 3);
    // TestRepeatChars(3, "Hello", 0);
    // TestRepeatChars(3, "Hello", 5);

    // TestIndexes();
    // AvmString s = AvmStringFrom("Hello World!");
    // TestReplaceAll(&s);
    // TestUpperLower(&s);
    // TestIsEmpty(&s);
    // TestUnsafeDestruct(&s);
    // TestToString(&s);
    // TestClone(&s);
    // TestReplaceN(&s);
    // TestReplaceLastN(&s);
    // TestReplace(&s);
    // TestReplaceLast(&s);
    // TestStartsWithStr(&s);
    // TestStartsWithChar(&s);
    // TestEndsWithStr(&s);
    // TestEndsWithChar(&s);
    // TestContainsStr(&s);
    // TestContainsChar(&s);
    // TestReverse(&s);
    // TestClear(&s);
    // TestErase(&s);
    // TestUnsafeSetLength(&s);
    // TestCharAt(&s);
}
