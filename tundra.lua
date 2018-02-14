local GlobExtension = require("tundra.syntax.glob")

Build {
  Env = {
    CPPDEFS = {
      { "TARGET_PC_DEV_DEBUG"; Config = "win64-*-debug-*" }
    },
  },
  Units = function ()
    -- Recursively globs for source files relevant to current build-id
    local function SourceGlob(dir)
        return FGlob {
            Dir = dir,
            Extensions = { ".c", ".cpp", ".s", ".asm" },
            Filters = {
            }
        }
    end

    local lib = StaticLibrary {
      Name = "xbase",
      Config = "*-*-*-static",
      Sources = { SourceGlob("source/main/cpp") },
      Includes = { "source/main/include/" },
    }

    local test = Program {
      Name = "xbase_test",
      Depends = { lib },
      Sources = { SourceGlob("source/test/cpp") },
      Includes = { "source/main/include/", "source/test/include/" },
    }

    Default(lib)

  end,
  Configs = {
    {
      Name = "macosx-gcc",
      DefaultOnHost = "macosx",
      Tools = { "gcc" },
    },
    {
      Name = "linux-gcc",
      DefaultOnHost = "linux",
      Tools = { "gcc" },
    },
    {
      Name = "win64-msvc",
      DefaultOnHost = "windows",
      Tools = { "msvc-vs2017" },
    },
  },

  Variants = { "debug", "release" },
  SubVariants = { "static" },
}

