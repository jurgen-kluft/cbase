const std = @import("std");
const Builder = std.build.Builder;
const LibExeObjStep = std.build.LibExeObjStep;
const ArrayList = std.ArrayList;

pub fn build(b: *std.build.Builder) !void {
    const target = b.standardTargetOptions(.{});
    const mode = b.standardReleaseOptions();

    const cbaselib = b.addStaticLibrary("cbase", null);
    cbaselib.setTarget(target);
    cbaselib.setBuildMode(mode);

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

    cbaselib.defineCMacro("TARGET_RELEASE", null);
    cbaselib.defineCMacro("TARGET_TEST", null);
    cbaselib.defineCMacro("NDEBUG", null);

    if (cbaselib.target.isWindows()) {
        cbaselib.defineCMacro("TARGET_PC", null);
        try coreflags.append("-DTARGET_PC");
    } else if (cbaselib.target.isDarwin()) {
        cbaselib.defineCMacro("TARGET_MAC", null);
        try coreflags.append("-DTARGET_MAC");
    }

    cbaselib.linkLibCpp();
    cbaselib.addIncludePath("source/main/include");
    cbaselib.force_pic = true;

    try addSourceFilesFrom(b, cbaselib, "source/main/cpp", coreflags.toOwnedSlice());

    const cunittestlib = b.addStaticLibrary("cunittest", null);
    cunittestlib.setTarget(target);
    cunittestlib.setBuildMode(mode);
    cunittestlib.linkLibCpp();
    cunittestlib.addIncludePath("../cunittest/source/main/include");
    cunittestlib.force_pic = true;

    cunittestlib.defineCMacro("TARGET_RELEASE", null);
    cunittestlib.defineCMacro("TARGET_TEST", null);
    cunittestlib.defineCMacro("NDEBUG", null);
    if (cunittestlib.target.isWindows()) {
        cunittestlib.defineCMacro("PLATFORM_PC", null);
    } else if (cunittestlib.target.isDarwin()) {
        cunittestlib.defineCMacro("PLATFORM_MAC", null);
    }

    try addSourceFilesFrom(b, cunittestlib, "../cunittest/source/main/cpp", coreflags.toOwnedSlice());
    try addSourceFilesFrom(b, cunittestlib, "../cunittest/source/main/cpp/entry", coreflags.toOwnedSlice());

    const cbase_test = b.addExecutable("cbase-test", null);
    cbase_test.setTarget(target);
    cbase_test.setBuildMode(mode);
    cbase_test.install();
    cbase_test.linkLibCpp();
    cbase_test.linkLibrary(cbaselib);
    cbase_test.linkLibrary(cunittestlib);
    cbase_test.addIncludePath("source/main/include");
    cbase_test.addIncludePath("source/test/include");
    cbase_test.addIncludePath("../cunittest/source/main/include");
    try addSourceFilesFrom(b, cbase_test, "source/test/cpp", coreflags.toOwnedSlice());

}

// Add all .cpp files from the given path
fn addSourceFilesFrom(b: *Builder, step: *LibExeObjStep, path: []const u8, CxxArgs: []const []const u8) !void {
    var dir = std.fs.cwd().openIterableDir(path, .{}) catch {
        std.debug.print("Unable to open path '{s}'!", .{path});
        return;
    };

    var it = dir.iterate();
    while (try it.next()) |file| {
        if (file.kind != .File or !std.mem.endsWith(u8, file.name, ".cpp")) {
            continue;
        }
        const name = try std.fmt.allocPrint(b.allocator, "{s}/{s}", .{ path, file.name });
        step.addCSourceFile(name, CxxArgs);
    }
}
