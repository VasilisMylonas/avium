#include <avium/testing.h>
#include <avium/options.h>
#include <avium/fmt.h>

static object TestInit(int argc, str argv[]) {
    return AvmOptionParserNew(argc, argv);
}

// AvmOptionParserAddStandardOptions(state);
static void TestFini(object state) {
    AvmObjectDestroy(state);
    free(state);
}

// TEST
static void TestUsage(object state) {
    AvmOptionParserAddOption(state, "output", "Set the output file.",
                             OK_STRING);
    AvmOptionParserAddOption(state, "help", "Show this help prompt.", OK_FLAG);
    AvmOptionParserAddOptionEx(state, "version", 'v',
                               "Show version information.", OK_FLAG);
    AvmOptionParserShowUsage(state, "Do something useful.");
}
