// Compatible with Zig Version 0.11.0
const std = @import("std");
const fs = std.fs;
const fmt = std.fmt;
const heap = std.heap;
const mem = std.mem;
const ArrayList = std.ArrayList;
const Compile = std.Build.Step.Compile;
const ConfigHeader = std.Build.Step.ConfigHeader;
const Mode = std.builtin.Mode;
const Target = std.Target;
const ResolvedTarget = std.Build.ResolvedTarget;

const Maker = struct {
    builder: *std.Build,
    target: ResolvedTarget,
    optimize: Mode,
    enable_lto: bool,

    include_dirs: ArrayList([]const u8),
    cflags: ArrayList([]const u8),
    cxxflags: ArrayList([]const u8),
    objs: ArrayList(*Compile),

    fn addInclude(m: *Maker, dir: []const u8) !void {
        try m.include_dirs.append(dir);
    }
    fn addProjectInclude(m: *Maker, path: []const []const u8) !void {
        try m.addInclude(try m.builder.build_root.join(m.builder.allocator, path));
    }
    fn addCFlag(m: *Maker, flag: []const u8) !void {
        try m.cflags.append(flag);
    }
    fn addCxxFlag(m: *Maker, flag: []const u8) !void {
        try m.cxxflags.append(flag);
    }
    fn addFlag(m: *Maker, flag: []const u8) !void {
        try m.addCFlag(flag);
        try m.addCxxFlag(flag);
    }

    fn init(builder: *std.Build) !Maker {
        const target = builder.standardTargetOptions(.{});
        const zig_version = @import("builtin").zig_version_string;
        const commit_hash = try std.ChildProcess.run(
            .{ .allocator = builder.allocator, .argv = &.{ "git", "rev-parse", "HEAD" } },
        );
        try std.fs.cwd().writeFile("source/test/cpp/build-info.cpp", builder.fmt(
            \\int BUILD_NUMBER = {};
            \\char const *BUILD_COMMIT = "{s}";
            \\char const *BUILD_COMPILER = "Zig {s}";
            \\char const *BUILD_TARGET = "{s}";
            \\
        , .{ 0, commit_hash.stdout[0 .. commit_hash.stdout.len - 1], zig_version, try target.query.allocDescription(builder.allocator) }));
        var m = Maker{
            .builder = builder,
            .target = target,
            .optimize = builder.standardOptimizeOption(.{}),
            .enable_lto = false,
            .include_dirs = ArrayList([]const u8).init(builder.allocator),
            .cflags = ArrayList([]const u8).init(builder.allocator),
            .cxxflags = ArrayList([]const u8).init(builder.allocator),
            .objs = ArrayList(*Compile).init(builder.allocator),
        };

        try m.addCFlag("-std=c11");
        try m.addCxxFlag("-std=c++11");
        try m.addProjectInclude(&.{"source/main/include"});
        return m;
    }

    fn addSourceFilesFrom(m: *const Maker, step: *Compile, path: []const u8) !void {
        const root_path = m.builder.pathFromRoot(".");
        var cwd = try fs.openDirAbsolute(root_path, .{});
        defer cwd.close();

        const src_path = path;
        const src_dir = try fs.Dir.openDir(cwd, src_path, .{ .iterate = true, .no_follow = true });

        const allocator = heap.page_allocator;

        const flags = &.{
            "-fvisibility=hidden",
            "-fno-strict-aliasing",
            "-fno-strict-overflow",
            "-fwrapv",
            "-flax-vector-conversions",
        };

        var walker = try src_dir.walk(allocator);
        while (try walker.next()) |entry| {
            const name = entry.basename;
            if (mem.endsWith(u8, name, ".c")) {
                const full_path = try fmt.allocPrint(allocator, "{s}/{s}", .{ src_path, entry.path });
                step.addCSourceFiles(.{
                    .files = &.{full_path},
                    .flags = flags,
                });
            } else if (mem.endsWith(u8, name, ".cpp")) {
                const full_path = try fmt.allocPrint(allocator, "{s}/{s}", .{ src_path, entry.path });
                step.addCSourceFiles(.{
                    .files = &.{full_path},
                    .flags = flags,
                });
            }
        }
    }

    fn exe(m: *const Maker, name: []const u8, src: []const u8, deps: []const *Compile) *Compile {
        const e = m.builder.addExecutable(.{ .name = name, .target = m.target, .optimize = m.optimize });
        //e.addCSourceFiles(&.{src}, m.cxxflags.items);
        e.addCSourceFiles(.{ .files = &.{src}, .flags = m.cxxflags.items });

        for (deps) |d| e.addObject(d);
        for (m.objs.items) |o| e.addObject(o);
        for (m.include_dirs.items) |i| e.addIncludePath(.{ .path = i });

        // https://github.com/ziglang/zig/issues/15448
        if (m.target.result.abi == .msvc) {
            e.linkLibC(); // need winsdk + crt
        } else {
            // linkLibCpp already add (libc++ + libunwind + libc)
            e.linkLibCpp();
        }
        m.builder.installArtifact(e);
        e.want_lto = m.enable_lto;
        return e;
    }
};

pub fn build(b: *std.Build) !void {
    var make = try Maker.init(b);
    var target = make.target;
    make.enable_lto = b.option(bool, "lto", "Enable LTO optimization, (default: false)") orelse false;

    var os_tagname = @tagName(target.result.os.tag);

    switch (target.result.os.tag) {
        .macos => {
            os_tagname = "darwin";
            target.query.os_version_min = std.zig.CrossTarget.OsVersion{ .semver = .{ .major = 11, .minor = 0, .patch = 0 } };
        },
        .windows => {
            target.query.os_version_min = std.zig.CrossTarget.OsVersion{
                // Windows 1809
                // Minimum version for a syscall related to bun.sys.renameat
                // if you update this please update install.ps1
                .windows = .win10_rs5,
            };
        },
        .linux => {
            target.query.setGnuLibCVersion(2, 27, 0);
        },
        else => {},
    }

    var coreflags = ArrayList([]const u8).init(b.allocator);
    defer coreflags.deinit();

    try coreflags.append("-Wstrict-prototypes");
    try coreflags.append("-Wwrite-strings");
    try coreflags.append("-Wno-missing-field-initializers");
    try coreflags.append("-Wno-shift-count-overflow");
    try coreflags.append("-Wno-unused-parameter");
    try coreflags.append("-DTARGET_RELEASE");
    try coreflags.append("-DTARGET_TEST");
    try coreflags.append("-DNDEBUG");
    if (target.result.os.tag == .windows) {
        try coreflags.append("-DTARGET_PC");
    } else if (target.result.os.tag == .macos) {
        try coreflags.append("-DTARGET_MAC");
    }

    // -------------------------------------------------------------------------------------------------------------
    // -------------------------------------------------------------------------------------------------------------
    const ccorelib = b.addStaticLibrary(.{ .name = "ccore", .target = target, .optimize = make.optimize });

    ccorelib.defineCMacro("TARGET_RELEASE", null);
    ccorelib.defineCMacro("TARGET_TEST", null);
    ccorelib.defineCMacro("NDEBUG", null);
    if (target.result.os.tag == .windows) {
        ccorelib.defineCMacro("TARGET_PC", null);
    } else if (target.result.os.tag == .macos) {
        ccorelib.defineCMacro("TARGET_MAC", null);
    }

    const ccore_path = "../ccore/";

    ccorelib.linkLibCpp();
    ccorelib.addIncludePath(b.path(ccore_path ++ "source/main/include"));

    try make.addSourceFilesFrom(ccorelib, ccore_path ++ "source/main/cpp");

    // -------------------------------------------------------------------------------------------------------------
    // -------------------------------------------------------------------------------------------------------------
    const cbaselib = b.addStaticLibrary(.{ .name = "cbase", .target = target, .optimize = make.optimize });

    cbaselib.defineCMacro("TARGET_RELEASE", null);
    cbaselib.defineCMacro("TARGET_TEST", null);
    cbaselib.defineCMacro("NDEBUG", null);
    if (target.result.os.tag == .windows) {
        cbaselib.defineCMacro("TARGET_PC", null);
    } else if (target.result.os.tag == .macos) {
        cbaselib.defineCMacro("TARGET_MAC", null);
    }

    cbaselib.linkLibCpp();
    cbaselib.addIncludePath(b.path(ccore_path ++ "source/main/include"));
    cbaselib.addIncludePath(b.path("source/main/include"));

    try make.addSourceFilesFrom(cbaselib, "source/main/cpp");

    // -------------------------------------------------------------------------------------------------------------
    // -------------------------------------------------------------------------------------------------------------
    const cunittestlib = b.addStaticLibrary(.{ .name = "cunittest", .target = target, .optimize = make.optimize });

    cunittestlib.defineCMacro("TARGET_RELEASE", null);
    cunittestlib.defineCMacro("TARGET_TEST", null);
    cunittestlib.defineCMacro("NDEBUG", null);
    if (target.result.os.tag == .windows) {
        cunittestlib.defineCMacro("TARGET_PC", null);
    } else if (target.result.os.tag == .macos) {
        cunittestlib.defineCMacro("TARGET_MAC", null);
    }

    const cunittest_path = "../cunittest/";

    cunittestlib.linkLibCpp();
    cunittestlib.addIncludePath(b.path(cunittest_path ++ "source/main/include"));

    try make.addSourceFilesFrom(cunittestlib, cunittest_path ++ "source/main/cpp");
    try make.addSourceFilesFrom(cunittestlib, cunittest_path ++ "source/main/cpp/entry");

    // -------------------------------------------------------------------------------------------------------------
    // -------------------------------------------------------------------------------------------------------------
    const cbase_test = make.exe("cbase-test", "source/test/cpp/test_main.cpp", &.{});

    cbase_test.linkLibCpp();
    cbase_test.linkLibrary(ccorelib);
    cbase_test.linkLibrary(cbaselib);
    cbase_test.linkLibrary(cunittestlib);
    cbase_test.addIncludePath(b.path("source/main/include"));
    cbase_test.addIncludePath(b.path("source/test/include"));
    cbase_test.addIncludePath(b.path(ccore_path ++ "source/main/include"));
    cbase_test.addIncludePath(b.path(cunittest_path ++ "source/main/include"));

    cbase_test.defineCMacro("TARGET_RELEASE", null);
    cbase_test.defineCMacro("TARGET_TEST", null);
    cbase_test.defineCMacro("NDEBUG", null);
    if (target.result.os.tag == .windows) {
        cbase_test.defineCMacro("TARGET_PC", null);
    } else if (target.result.os.tag == .macos) {
        cbase_test.defineCMacro("TARGET_MAC", null);
    }

    try make.addSourceFilesFrom(cbase_test, "source/test/cpp");
}
