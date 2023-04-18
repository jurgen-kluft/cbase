package main

import (
	cpkg "github.com/jurgen-kluft/cbase/package"
	ccode "github.com/jurgen-kluft/ccode"
)

func main() {
	ccode.Init()
	ccode.Generate(cpkg.GetPackage())
	ccode.GenerateFiles()
}
