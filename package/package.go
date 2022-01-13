package xbase

// GetPackage returns the package object of 'xbase'
// How could we put the base URL github.com and the
// user 'jurgen-kluft' as a configuration var?

import (
	"github.com/jurgen-kluft/xcode/denv"
	xunittest "github.com/jurgen-kluft/xunittest/package"
)

func GetPackage() *denv.Package {
	name := "xbase"

	// Dependencies
	unittestpkg := xunittest.GetPackage()

	// The main (xbase) package
	mainpkg := denv.NewPackage(name)
	mainpkg.AddPackage(unittestpkg)

	// library
	mainlib := denv.SetupDefaultCppLibProject(name, "github.com\\jurgen-kluft\\"+name)
	mainlib.Dependencies = append(mainlib.Dependencies, unittestpkg.GetMainLib())

	// unittest project
	maintest := denv.SetupDefaultCppTestProject(name+"_test", "github.com\\jurgen-kluft\\"+name)
	maintest.Dependencies = append(maintest.Dependencies, unittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
