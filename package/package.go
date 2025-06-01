package cbase

import (
	denv "github.com/jurgen-kluft/ccode/denv"
	ccore "github.com/jurgen-kluft/ccore/package"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

const (
	repo_path = "github.com\\jurgen-kluft"
	repo_name = "cbase"
)

func GetPackage() *denv.Package {
	name := repo_name

	// dependencies
	corepkg := ccore.GetPackage()
	unittestpkg := cunittest.GetPackage()

	// main package
	mainpkg := denv.NewPackage(repo_path, repo_name)

	// main library
	mainlib := denv.SetupCppLibProject(mainpkg, name)
	mainlib.AddDependencies(corepkg.GetMainLib()...)

	// main test library
	testlib := denv.SetupCppTestLibProject(mainpkg, name)
	testlib.AddDependencies(corepkg.GetTestLib()...)

	// unittest for this package
	maintest := denv.SetupCppTestProject(mainpkg, name)
	maintest.AddDependencies(unittestpkg.GetMainLib()...)
	maintest.AddDependency(testlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddTestLib(testlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
