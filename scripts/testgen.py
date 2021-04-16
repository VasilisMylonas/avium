#!/usr/bin/env python3

import sys
import os
import argparse

runner_template = """// Auto-generated
#include <avium/runtime.h>
@FILE@

int main(int argc, str argv[])
{
    AvmEnableExceptions();
@TESTS@
}
"""


def get_function_name(declaration: str) -> str:
    if "__test " not in declaration:
        return ""

    name: str = declaration[0:declaration.index("(")]
    name: str = name[name.rindex(" ") + 1:]
    return name


def get_function_names(declarations: list) -> list:
    result = []
    for declaration in declarations:
        name = get_function_name(declaration)
        if name != "":
            result.append(name)
    return result


def create_test_runner(names: list, contents: str) -> str:
    calls: str = ""

    for name in names:
        calls += f"    {name}(TestInit(argc, argv));\n"

    return runner_template.replace("@FILE@", contents).replace("@TESTS@", calls)


def parse_args():
    parser = argparse.ArgumentParser(description="Automatically run C tests.")
    parser.add_argument("-o", "--output", type=str, default="stdout")
    parser.add_argument("file", type=str)

    return parser.parse_args()


def main() -> None:
    ns = parse_args()
    contents: list = []

    with open(ns.file, "r") as f:
        contents = f.readlines()

    contents = create_test_runner(
        get_function_names(contents), "".join(contents))

    if ns.output == "stdout":
        print(contents, end="")
    else:
        with open(ns.output, "w") as f:
            f.write(contents)


if __name__ == "__main__":
    main()
