package main

import (
	"github.com/jurgen-kluft/xbase/package"
	"github.com/jurgen-kluft/xcode"
)

func main() {
	xcode.Generate(xbase.GetPackage())
}
