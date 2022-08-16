package cbase

// GetPackage returns the package object of 'xbase'
// How could we put the base URL github.com and the
// user 'jurgen-kluft' as a configuration var?

import (
	"github.com/jurgen-kluft/ccode/denv"
	xunittest "github.com/jurgen-kluft/cunittest/package"
)

const (
	repo_path = "github.com\\jurgen-kluft\\"
	repo_name = "cbase"
)

func GetPackage() *denv.Package {
	name := repo_name

	// Dependencies
	unittestpkg := xunittest.GetPackage()

	// The main (xbase) package
	mainpkg := denv.NewPackage(name)

	// library
	mainlib := denv.SetupDefaultCppLibProject(name, repo_path+name)

	// unittest project
	maintest := denv.SetupDefaultCppTestProject(name+"_test", repo_path+name)
	maintest.Dependencies = append(maintest.Dependencies, unittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
