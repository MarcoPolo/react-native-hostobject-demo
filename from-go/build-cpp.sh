# Change this if not on a macOS platform
build() {
    GOPATH=`pwd`/../go
    CLANG_HOME=$ANDROID_HOME/ndk-bundle/toolchains/llvm/prebuilt/darwin-x86_64/bin
    ARCH=$1

    env \
    GOOS=$2 \
    GOARCH=$3 \
    CGO_ENABLED=1 \
    CC=$CLANG_HOME/$ARCH-clang \
    $CLANG_HOME/$ARCH-clang++ -Wl,-rpath=/data/data hello.cpp -o hello -L `pwd` -lgojni

}

# Very hacky for now
# build(clang-toolchain-name GOOS GOARCH jniLib-Arch)
build i686-linux-android16 android 386 x86
