local GlobExtension = require("tundra.syntax.glob")

Build {
  ReplaceEnv = {
    OBJECTROOT = "target",
  },
  Env = {
    CPPDEFS = {
      { "TARGET_PC_DEV_DEBUG", "TARGET_PC", "PLATFORM_64BIT"; Config = "win64-*-debug-*" },
      { "TARGET_MAC_DEV_DEBUG", "TARGET_MAC", "PLATFORM_64BIT"; Config = "macosx-*-debug-*" },
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
            }
        }
    end

    local xbase_inc = "source/main/include/"
    local xunittest_inc = "../xunittest/source/main/include/"

    local xbase_lib = StaticLibrary {
      Name = "xbase",
      Config = "*-*-*-static",
      Sources = { SourceGlob("source/main/cpp") },
      Includes = { xbase_inc },
    }

    local xunittest_lib = StaticLibrary {
      Name = "xunittest",
      Config = "*-*-*-static",
      Sources = { SourceGlob("../xunittest/source/main/cpp") },
      Includes = { xunittest_inc },
    }

    local unittest = Program {
      Name = "xbase_unittest",
      Depends = { xbase_lib, xunittest_lib },
      Sources = { SourceGlob("source/test/cpp") },
      Includes = { "source/test/include/", xbase_inc, xunittest_inc },
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

  Variants = { "debug", "release" },
  SubVariants = { "static" },
}

