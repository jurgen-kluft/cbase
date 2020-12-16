package xbase

import (
	"github.com/jurgen-kluft/xcode/denv"
	"github.com/jurgen-kluft/xunittest/package"
)

// GetPackage returns the package object of 'xbase'
func GetPackage() *denv.Package {
	// Dependencies
	unittestpkg := xunittest.GetPackage()

	// The main (xbase) package
	mainpkg := denv.NewPackage("xbase")
	mainpkg.AddPackage(unittestpkg)

	// 'xbase' library
	mainlib := denv.SetupDefaultCppLibProject("xbase", "github.com\\jurgen-kluft\\xbase")
	mainlib.Dependencies = append(mainlib.Dependencies, unittestpkg.GetMainLib())

	// 'xbase' unittest project
	maintest := denv.SetupDefaultCppTestProject("xbase_test", "github.com\\jurgen-kluft\\xbase")
	maintest.Dependencies = append(maintest.Dependencies, unittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
