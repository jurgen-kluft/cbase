package xbase

import (
	"github.com/jurgen-kluft/xbase/package"
	"github.com/jurgen-kluft/xcode/denv"
)

// GetProject returns the package object of 'xbase'
func GetProject() *denv.Project {
	xcode.SetupDefaultCppProject("xbase", "github.com\\jurgen-kluft")
	project.Dependencies = append(project.Dependencies, xbase.GetProject())

	return project
}
