#!/usr/bin/env python3

import sys
import os
import argparse


def remove_parentheses(s: str) -> str:
    return s.replace("(", "").replace(")", "")


def get_function_name(declaration: str) -> str:
    parts = declaration.strip().split()

    for i in range(0, len(parts)):
        if parts[i].startswith("("):
            return remove_parentheses(parts[i - 1])

        if "(" in parts[i]:
            index = parts[i].index("(")
            return parts[i][0:index]

    return ""


def get_tests(lines: list) -> list:
    tests = []
    isTest = False

    for line in lines:
        if isTest:
            name = get_function_name(line)
            if name != "":
                tests.append(name)
            isTest = False
            continue

        if line.startswith("// TEST"):
            isTest = True

    return tests


runner_template = """// Auto-generated
#include <avium/testing.h>
@FILE@

int main(void)
{
    object state = TestInit();

@TESTS@
    TestFini(state);
}
"""


def create_test_runner(tests: list, file_contents: str) -> str:
    contents = ""

    for test in tests:
        contents += f"    {test}(state);\n"

    return runner_template.replace("@FILE@", file_contents).replace("@TESTS@", contents)


def parse_args():
    parser = argparse.ArgumentParser(description="Automatically run C tests.")
    parser.add_argument("-o", "--output", type=str, default="stdout")
    parser.add_argument("file", type=str)

    return parser.parse_args()


def main():
    ns = parse_args()
    tests = []
    contents = ""

    with open(ns.file, "r") as f:
        file_contents = f.read()
        tests = get_tests(file_contents.split("\n"))
        contents = create_test_runner(tests, file_contents)

    if ns.output == "stdout":
        print(contents, end="")
    else:
        with open(ns.output, "w") as f:
            f.write(contents)


if __name__ == "__main__":
    main()
