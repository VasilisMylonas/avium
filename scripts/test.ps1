$BUILD_DIR = $ARGS[0]
$BUILD_TYPE = $ARGS[1]

Set-Location $BUILD_DIR
& "ctest.exe" -C $BUILD_TYPE
