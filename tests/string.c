#include <stdio.h>
#include <string.h>

#include "avium/avium.h"
#include "avium/testing.h"

int main() {
    AvmTestSetOutput(stdout);
    Plan(25);

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
    Ok(AvmStringAsPtr(s)[0] == 'H', "First char is 'H'.");
    Ok(AvmStringAsPtr(s)[23] == '!', "Last char is '!'.");
    Ok(AvmStringIndexOf(s, 'W') == 6, "First 'W' is at index 6.");
    Ok(AvmStringLastIndexOf(s, 'W') == 18, "Last 'W' is at index 18.");
    Ok(AvmStringCharAt(s, 2) == 'l', "Correct char returned.");
    Ok(AvmStringFind(s, "World") == 6, "First 'World' is at index 6.");
    Ok(AvmStringFindLast(s, "World") == 18, "Last 'World' is at index 18.");

    Ok(AvmStringIndexOf(s, 'x') == AVM_STRING_NPOS,
       "No first occurrence of 'x'.");

    Ok(AvmStringLastIndexOf(s, 'x') == AVM_STRING_NPOS,
       "No last occurrence of 'x'.");

    Ok(AvmStringFind(s, "xxl") == AVM_STRING_NPOS,
       "No first occurrence of 'xxl'.");

    Ok(AvmStringFindLast(s, "xxl") == AVM_STRING_NPOS,
       "No last occurrence of 'xxl'.");

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
