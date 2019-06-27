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
    CXX=$CLANG_HOME/$ARCH-clang++ \
    go build -buildmode=c-shared -v jsi-test/testnum
    mv testnum ../android/app/src/main/jniLibs/$4/libtestnum.so
}

build i686-linux-android16 android 386 x86
