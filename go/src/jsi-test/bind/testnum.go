package testnum

import "C"

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
