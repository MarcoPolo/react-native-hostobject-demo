package main

// #cgo CFLAGS: -I$ANDROID_HOME/ndk-bundle/sysroot/usr/include
// #include <jni.h>
import "C"
import "time"

var g int = 0

// TestNum returns a test number to be used in JSI
//export TestNum
func TestNum() int {
	return int(9001 + g)
}

// TestCb returns a test number to be used in JSI
//export TestCb
func TestCb() *C.char {
	time.Sleep(1 * time.Second)
	return C.CString("Hello from Go")
}

//export Java_com_testmodule_MainActivity_hi
func Java_com_testmodule_MainActivity_hi(env *C.JNIEnv, clazz C.jclass) {
	g = g + 1
}

func main() {
}
