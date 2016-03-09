package xbase

import (
	"github.com/jurgen-kluft/xcode/denv"
	"github.com/jurgen-kluft/xunittest/package"
)

// GetProject returns the package object of 'xbase'
func GetProject() *denv.Project {
	project := denv.SetupDefaultCppProject("xbase", "github.com\\jurgen-kluft")
	project.Dependencies = append(project.Dependencies, xunittest.GetProject())
	return project
}
