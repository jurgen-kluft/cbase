msbuild pom.targets /t:Construct /p:PackagePlatform=x64 /p:PackageIDE=VS2013 /p:PackageToolSet=v120
msbuild pom.targets /t:Install /p:PackagePlatform=x64 /p:PackageIDE=VS2013 /p:PackageToolSet=v120
