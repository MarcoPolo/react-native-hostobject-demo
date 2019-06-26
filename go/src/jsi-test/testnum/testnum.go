package main

import "C"

// TestNum returns a test number to be used in JSI
//export TestNum
func TestNum() int {
	return int(9001)
}

func main() {
}
