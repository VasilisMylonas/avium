$BUILD_DIR = $ARGS[0]
$BUILD_TYPE = $ARGS[1]

& "cmake.exe" -E make_directory $BUILD_DIR
& "cmake.exe" -S . -B $BUILD_DIR -DAVM_USE_GC=OFF -DCMAKE_BUILD_TYPE=$BUILD_TYPE
& "cmake.exe" --build $BUILD_DIR --config $BUILD_TYPE
