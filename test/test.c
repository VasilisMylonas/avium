#include <stdio.h>
#include <string.h>

#include "avium.h"
#include "avium/testing.h"

static void AvmTestString() {
    Plan(22);

    AvmString s = AvmString(20);
    Ok(s != NULL, "Object not null.");
    Ok(AvmStringGetCapacity(s) == 20, "Capacity is correct.");
    Ok(AvmStringGetLength(s) == 0, "Length is 0.");

    AvmDestroy(s);
    Pass("Destroyed successfully");

    s = AvmStringFrom("Hello World Wordy World!");
    Ok(s != NULL, "Object not null.");
    Ok(AvmStringGetCapacity(s) == AVM_STRING_GROWTH_FACTOR * 24,
       "Capacity is correct.");
    Ok(AvmStringGetLength(s) == 24, "Length is correct.");
    Ok(AvmStringAsPtr(s)[0] == 'H' && AvmStringAsPtr(s)[23] == '!',
       "Contents are correct.");
    Ok(AvmStringIndexOf(s, 'W') == 6, "Correct first index returned.");
    Ok(AvmStringLastIndexOf(s, 'W') == 18, "Correct last index returned.");
    Ok(AvmStringCharAt(s, 2) == 'l', "Correct char returned.");
    Ok(AvmStringFind(s, "World") == 6,
       "Correct first substring index returned.");
    Ok(AvmStringFindLast(s, "World") == 18,
       "Correct last substring index returned.");
    Ok(AvmStringIndexOf(s, 'x') == AVM_STRING_NPOS &&
           AvmStringLastIndexOf(s, 'x') == AVM_STRING_NPOS,
       "Npos returned if char not found.");
    Ok(AvmStringFind(s, "xxl") == AVM_STRING_NPOS &&
           AvmStringFindLast(s, "xxl") == AVM_STRING_NPOS,
       "Npos returned if substring not found.");

    Ok(AvmStringReplace(s, 'o', 'a') == 4, "Correct number of chars replaced.");
    Ok(strcmp(AvmStringAsPtr(s), "Hella Warld Wardy Warld!") == 0,
       "All chars replaced.");

    AvmStringToUpper(s);

    Ok(strcmp(AvmStringAsPtr(s), "HELLA WARLD WARDY WARLD!") == 0,
       "All chars are uppercase.");

    AvmStringToLower(s);

    Ok(strcmp(AvmStringAsPtr(s), "hella warld wardy warld!") == 0,
       "All chars are lowercase.");

    s = AvmStringAppend(s, ".com");

    Ok(strcmp(AvmStringAsPtr(s), "hella warld wardy warld!.com") == 0,
       "Chars appended correctly.");

    Todo("Concat works.");
    Todo("Cloned AvmString.");
}

int32_t main(int32_t argc, const char8_t** argv) {
    AvmOption option = AvmOption("avm-tap-output", 0);
    AvmOptional opt = AvmGetOption(argc, argv, option);

    if (AvmHasValue(opt)) {
        FILE* f = fopen((const char8_t*)opt, "w");
        AvmTestSetOutput(f);
        AvmTestString();
        fclose(f);
    }
}
