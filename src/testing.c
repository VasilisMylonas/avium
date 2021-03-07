#include "avium/testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t currentTest = 0;
static size_t testCount = 0;
static size_t skipCount = 0;
static size_t failedCount = 0;
static FILE* outputStream = NULL;

void AvmTestSetOutput(void* stream) { outputStream = stream; }

void Plan(size_t count) {
    testCount = count;
    fprintf(outputStream, "1..%zu\n\n", count);
}

static char8_t* Concat(const char8_t* s1, const char8_t* s2) {
    size_t length1 = strlen(s1);
    size_t length2 = strlen(s2);
    char8_t* buffer = malloc(length1 + length2 + 1);

    memcpy(buffer, s1, length1);
    memcpy(buffer + length1, s2, length2);

    buffer[length1 + length2] = '\0';

    return buffer;
}

void Skip(size_t count) { skipCount = count; }

void Note(const char8_t* message) {
    fprintf(outputStream, "Note: %s", message);
}

void Todo(const char8_t* description) {
    char8_t* s = Concat(description, " # TODO");
    Pass(s);
    free(s);
}

static void EndPlan() {
    // fprintf(outputStream, "Failed %zu/%zu tests. %.2lf%% okay.\n",
    // failedCount,
    //         testCount, 100.0 - (double)failedCount / (double)testCount *
    //         100);

    currentTest = 0;
    testCount = 0;
    skipCount = 0;
    failedCount = 0;
}

static void PrintTestStatus(bool expression, const char8_t* description) {
    const char8_t* formatString = "ok %zu %s %s\n";

    if (!expression) {
        formatString = "not ok %zu %s %s\n";
        failedCount++;
    }

    if (description == NULL) {
        fprintf(outputStream, formatString, currentTest, "", "");
    } else {
        fprintf(outputStream, formatString, currentTest, "-", description);
    }
}

void Ok(bool expression, const char8_t* description) {
    currentTest++;

    if (currentTest > testCount) {
        fprintf(outputStream, "# ERROR: Unexpected test.\n");
        exit(255);
    }

    if (skipCount != 0) {
        skipCount--;
        char8_t* s = Concat(description, " # SKIP");
        fprintf(outputStream, "ok %zu - %s\n", currentTest, s);
        free(s);
    } else {
        PrintTestStatus(expression, description);
    }

    if (currentTest == testCount) {
        EndPlan();
    }
}

void BailOut(const char8_t* reason) {
    if (reason == NULL) {
        fprintf(outputStream, "Bail out!\n");
    } else {
        fprintf(outputStream, "Bail out! %s\n", reason);
    }

    exit(255);
}

void Pass(const char8_t* description) { Ok(true, description); }

void Fail(const char8_t* description) { Ok(false, description); }
