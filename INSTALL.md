# Building from source

## Requirements

To build avium you need the following installed on your system:

- A C compiler such as clang or gcc.
- Preferably the latest version of CMake, which you can find [here](http://www.cmake.org).

### Optional tools

- [xunit-viewer](https://github.com/lukejpreston/xunit-viewer) **NOTE: xunit-viewer hangs after HTML generation.** To fix it, add the following after line 33 in xunit-viewer.js located in the package folder:

    ```javascript
    process.exit(0)
    ```

- [Doxygen](https://www.doxygen.nl)

    ```sh
        sudo apt install doxygen
    ```

- [Sphinx](https://www.sphinx-doc.org)

    ```sh
        sudo pip3 install sphinx
    ```

- [Breathe](https://breathe.readthedocs.io)

    ```sh
        sudo pip3 install breathe
    ```

- ReadTheDocs theme for Sphinx

    ```sh
        sudo pip3 install sphinx-rtd-theme
    ```



## Building

### Linux

1. Clone this repository.

    ```sh
    git clone https://github.com/VasilisMylonas/avium.git
    ```

2. Go to where you cloned the github repo.

    ```sh
    cd avium
    ```

3. Create a directory named "build".

    ```sh
    mkdir build
    ```

4. Generate the build files using cmake.

    ```sh
    cmake -S . -B ./build/
    ```

5. Go to the "build" directory.

    ```sh
    cd build
    ```

6. Build the library.

    ```sh
    make
    ```

After that you can find the created shared objects (libavm and libavm-static) in the "lib" directory.

### Windows

TODO ¯\\_(ツ)_/¯