#include <avium/testing.h>
#include <avium/options.h>

static object TestInit(int argc, str argv[]) {
    return AvmOptionParserNew(argc, argv);
}

__test TestUsage(object state) {
    AvmOptionParserAddOption(state, "output", "Set the output file.",
                             ValueKindStr);
    AvmOptionParserAddOption(state, "help", "Show this help prompt.",
                             ValueKindBool);
    AvmOptionParserAddOptionEx(state, "version", 'v',
                               "Show version information.", ValueKindBool);

    AvmOptionParserShowUsage(state, "Do something useful.");
}
