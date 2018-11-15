local GlobExtension = require("tundra.syntax.glob")
Build {
	ReplaceEnv = {
		OBJECTROOT = "target",
	},
	Env = {
		CPPDEFS = {
			{ "TARGET_PC", "PLATFORM_64BIT"; Config = "win64-*-*-*" },
			{ "TARGET_MAC", "PLATFORM_64BIT"; Config = "macosx-*-*-*" },
			{ "TARGET_DEV_DEBUG"; Config = "*-*-debug-*" },
			{ "TARGET_DEV_RELEASE"; Config = "*-*-release-*" },
			{ "TARGET_TEST"; Config = "*-*-*-test" },
		},
	},
	Units = function ()
		-- Recursively globs for source files relevant to current build-id
		local function SourceGlob(dir)
			return FGlob {
				Dir = dir,
				Extensions = { ".c", ".cpp", ".s", ".asm" },
				Filters = {
					{ Pattern = "_win32"; Config = "win64-*-*" },
					{ Pattern = "_mac"; Config = "macosx-*-*" },
					{ Pattern = "_test"; Config = "*-*-*-test" },
				}
			}
		end
		local xbase_library = StaticLibrary {
			Name = "xbase",
			Config = "*-*-*-*",
			Sources = { SourceGlob("source/main/cpp") },
			Includes = { "..//xbase/source/main/include","..//xunittest/source/main/include" },
		}
		local xunittest_library = StaticLibrary {
			Name = "xunittest",
			Config = "*-*-*-test",
			Sources = { SourceGlob("../xunittest/source/main/cpp") },
			Includes = { "..//xunittest/source/main/include" },
		}
		local xentry_library = StaticLibrary {
			Name = "xentry",
			Config = "*-*-*-*",
			Sources = { SourceGlob("../xentry/source/main/cpp") },
			Includes = { "..//xentry/source/main/include" },
		}
		local unittest = Program {
			Name = "xbase_test",
			Config = "*-*-*-test",
			Sources = { SourceGlob("source/test/cpp") },
			Includes = { "source/main/include","source/test/include","..//xunittest/source/main/include","..//xentry/source/main/include","..//xbase/source/main/include" },
			Depends = { xbase_library,xunittest_library,xentry_library },
		}
		Default(unittest)
	end,
	Configs = {
		Config {
			Name = "macosx-clang",
			Env = {
			PROGOPTS = { "-lc++" },
			CXXOPTS = {
				"-std=c++11",
				"-arch x86_64",
				"-Wno-new-returns-null",
				"-Wno-missing-braces",
				"-Wno-unused-function",
				"-Wno-unused-variable",
				"-Wno-unused-result",
				"-Wno-write-strings",
				"-Wno-c++11-compat-deprecated-writable-strings",
				"-Wno-null-dereference",
				"-Wno-format",
				"-fno-strict-aliasing",
				"-fno-omit-frame-pointer",
			},
		},
		DefaultOnHost = "macosx",
		Tools = { "clang" },
		},
		Config {
			ReplaceEnv = {
				OBJECTROOT = "target",
			},
			Name = "linux-gcc",
			DefaultOnHost = "linux",
			Tools = { "gcc" },
		},
		Config {
			ReplaceEnv = {
				OBJECTROOT = "target",
			},
			Name = "win64-msvc",
			Env = {
				PROGOPTS = { "/SUBSYSTEM:CONSOLE" },
				CXXOPTS = { },
			},
			DefaultOnHost = "windows",
			Tools = { "msvc-vs2017" },
		},
	},
	SubVariants = { "dev", "test" },
}
