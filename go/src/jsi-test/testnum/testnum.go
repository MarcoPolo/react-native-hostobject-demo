package main

// #cgo CFLAGS: -I$ANDROID_HOME/ndk-bundle/sysroot/usr/include
import "C"
import "time"

var g int = 0

// TestNum returns a test number to be used in JSI
//export TestNum
func TestNum() int {
	return int(9001 + g)
}

// TestStr returns a test number to be used in JSI
//export TestStr
func TestStr() *C.char {
	return C.CString("Hello from Go")
}

// TestCb returns a test number to be used in JSI
//export TestCb
func TestCb() *C.char {
	time.Sleep(1 * time.Second)
	return C.CString("Hello from Go")
}

func main() {
}
