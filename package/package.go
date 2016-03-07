package xbase

import "github.com/jurgen-kluft/xcode/denv"

// GetProject returns the package object of 'xbase'
func GetProject() *denv.Project {
	project := denv.SetupDefaultCppProject("xbase", "github.com\\jurgen-kluft")

	return project
}
