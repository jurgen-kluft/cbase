package cbase

import (
	ccore "github.com/jurgen-kluft/ccore/package"
	cunittest "github.com/jurgen-kluft/cunittest/package"
	denv "github.com/jurgen-kluft/go-ide/denv"
)

const (
	repo_path = "github.com\\jurgen-kluft"
	repo_name = "cbase"
)

func GetPackage() *denv.Package {
	// dependencies
	core_pkg := ccore.GetPackage()
	unittest_pkg := cunittest.GetPackage()

	// main package
	main_pkg := denv.NewPackage(repo_path, repo_name)
	main_pkg.AddPackage(core_pkg)
	main_pkg.AddPackage(unittest_pkg)

	// main library
	mainlib := denv.SetupCppLibProject(main_pkg, repo_name)
	mainlib.AddDependencies(core_pkg.GetMainLib())

	// main test library
	testlib := denv.SetupCppTestLibProject(main_pkg, repo_name)
	testlib.AddDependencies(core_pkg.GetTestLib())

	// unittest for this package
	maintest := denv.SetupCppTestProject(main_pkg, repo_name)
	maintest.AddDependencies(unittest_pkg.GetTestLib())
	maintest.AddDependency(testlib)

	main_pkg.AddMainLib(mainlib)
	main_pkg.AddTestLib(testlib)
	main_pkg.AddUnittest(maintest)

	return main_pkg
}
