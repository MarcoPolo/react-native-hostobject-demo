package main

// #cgo CFLAGS: -I/Library/Java/JavaVirtualMachines/jdk-12.0.1.jdk/Contents/Home/include
// #cgo CFLAGS: -I/Library/Java/JavaVirtualMachines/jdk-12.0.1.jdk/Contents/Home/include/darwin
// #include <jni.h>
import "C"

var g int = 0

// TestNum returns a test number to be used in JSI
//export TestNum
func TestNum() int {
	return int(9001 + g)
}

//export Java_com_testmodule_MainActivity_hi
func Java_com_testmodule_MainActivity_hi(env *C.JNIEnv, clazz C.jclass) {
	g = g + 1
}

func main() {
}
