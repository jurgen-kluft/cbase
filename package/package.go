package cbase

import (
	denv "github.com/jurgen-kluft/ccode/denv"
	ccore "github.com/jurgen-kluft/ccore/package"
	cunittest "github.com/jurgen-kluft/cunittest/package"
)

const (
	repo_path = "github.com\\jurgen-kluft\\"
	repo_name = "cbase"
)

func GetPackage() *denv.Package {
	name := repo_name

	// Dependencies
	corepkg := ccore.GetPackage()
	unittestpkg := cunittest.GetPackage()

	// The main (cbase) package
	mainpkg := denv.NewPackage(name)

	// library
	mainlib := denv.SetupDefaultCppLibProject(name, repo_path+name)
	mainlib.Dependencies = append(mainlib.Dependencies, corepkg.GetMainLib())

	// unittest project
	maintest := denv.SetupDefaultCppTestProject(name+"_test", repo_path+name)
	maintest.Dependencies = append(maintest.Dependencies, unittestpkg.GetMainLib())
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
