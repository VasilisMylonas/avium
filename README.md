# Avium

![Code factor badge](https://img.shields.io/codefactor/grade/github/VasilisMylonas/avium)
![Discord server badge](https://img.shields.io/discord/810959563469357057)
![Coverage Status](https://coveralls.io/repos/github/VasilisMylonas/avium/badge.svg?branch=main)
![Workflow status badge](https://img.shields.io/github/workflow/status/VasilisMylonas/avium/Main)

## Introduction 📖

Avium is an open source, multi-platform library that aims to fill in many missing features in the C language while remaining simple and understandable. If you are looking for an intuitive and object-oriented utility library, then Avium might be for you.

Documentation about the library is available [here](https://vasilismylonas.github.io/avium) and documentation comments are provided in the header files. You can also check out the examples present in the `examples` directory.

For more information about the project, join us on [our Discord server](https://discord.gg/ntcjbMbVts)! If you have any questions, feel free to ask!

You may also wish to check out the following dashboards:

- [Code Quality](https://www.codefactor.io/repository/github/vasilismylonas/avium)
- [Coverage](https://coveralls.io/github/VasilisMylonas/avium)

## Key Features ⌨️

- Resizable, heap allocated strings.
- Program argument parsing and help prompts.
- Runtime type information.
- Class and interface types, inheritance and virtual functions.
- Generic result and optional types.
- Fixed size generic arrays.
- Testing utilities.
- Intuitive formatted IO.
- Garbage collecting (with support from libgc).

## Why use this over C++

### Simplicity

C++ has a lot of features. Some are useful, some are not, and some are rather bad. Over the years C++ has increased a lot in complexity, making it hard to use, or confusing for some people.

This library tries to stay light and simple so that it can be easily integrated in any C project. We try to make the API as self-explaining as possible and want to keep it portable across operating systems.

### C library - stable ABI

C++ also has no stable ABI making it harder to interface with in higher level programming languages.

Because C is the lingua-franca of the programming world, an object-oriented system written in C is much easier to interface with higher level languages.

### Control

C++ gives you little to no control over how stuff like vtables, and method overloading work, or about the internal layout of a class.

With this library you have direct control over all of these, and can tweak it as you see necessary.

## Building 🔨

### Prerequisites

- A C11-compatible compiler such as Clang or GCC and MSVC for Windows.
- [CMake](https://cmake.org/) version 3.5.0 or newer.
- [Python 3](https://www.python.org/) with pip.
- A Bash compatible shell or Powershell on Windows.

For building the documentation, the following are also needed:

- [Doxygen](https://doxygen.nl)
- [Sphinx](https://github.com/sphinx-doc/sphinx)
- [Breathe](https://github.com/michaeljones/breathe)
- sphinx-rtd-theme

### Building the project

``` bash
# Clone this repository
$ git clone https://github.com/VasilisMylonas/avium.git

# Go into the repository
$ cd avium

# Configure
$ cmake -S . -B ./build

# Build
$ cmake --build ./build --config Release

# If you don't want garbage collection you must run this command instead:
$ cmake --build ./build -DAVM_USE_GC=OFF --config Release

# Test
$ cmake --build ./build --target test
```

## Supported Platforms 🏗️

Currently the following configurations are supported:

| Architecture | Operating System | Compiler |
| ------------ | ---------------- | -------- |
| x86_64       | Ubuntu           | GCC      |
| x86_64       | Ubuntu           | Clang    |
| x86_64       | Windows          | MSVC     |
| x86_64       | MacOS            | GCC      |

## Contributing 🤝

If you are interested in contributing, please take the time to read [CONTRIBUTING.md](./CONTRIBUTING.md). You should also take a look at the [code of conduct](./CODE_OF_CONDUCT.md).

## License 📕

Avium is licensed under version 3 of the GNU Lesser General Public License. See [COPYING](./COPYING) and [COPYING.LESSER](./COPYING.LESSER) for more information.
