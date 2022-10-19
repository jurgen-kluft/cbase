const std = @import("std");
const ArrayList = std.ArrayList;

pub fn build(b: *std.build.Builder) void {
    const target = b.standardTargetOptions(.{});
    const mode = b.standardReleaseOptions();

    const cbaselib = b.addStaticLibrary("cbase", null);
    cbaselib.setTarget(target);
    cbaselib.setBuildMode(mode);

    var coreflags = ArrayList([]const u8).init(b.allocator);
    defer coreflags.deinit();

    coreflags.append("strict-prototypes");
    try coreflags.append("write-strings");
    try coreflags.append("no-missing-field-initializers");
    try coreflags.append("no-shift-count-overflow");
    try coreflags.append("no-unused-parameter");

    try coreflags.append("TARGET_RELEASE");
    try coreflags.append("TARGET_TEST");
    try coreflags.append("NDEBUG");

    if (cbaselib.target.isWindows()) {    
        try coreflags.append("PLATFORM_PC");
    } else if (cbaselib.target.isDarwin()) {
        try coreflags.append("PLATFORM_MAC");
    }

    cbaselib.linkLibCpp();
    cbaselib.addIncludeDir("source/main/include");
    cbaselib.force_pic = true;
    cbaselib.addCSourceFiles(&.{
        "source/main/cpp/c_allocator.cpp",
        "source/main/cpp/c_allocator_system_mac.cpp",
        "source/main/cpp/c_allocator_system_win32.cpp",
        "source/main/cpp/c_base.cpp",
        "source/main/cpp/c_binary_search.cpp",
        "source/main/cpp/c_btree.cpp",
        "source/main/cpp/c_buffer.cpp",
        "source/main/cpp/c_console.cpp",
        "source/main/cpp/c_console_mac.cpp",
        "source/main/cpp/c_console_win32.cpp",
        "source/main/cpp/c_context.cpp",
        "source/main/cpp/c_darray.cpp",
        "source/main/cpp/c_debug.cpp",
        "source/main/cpp/c_guid.cpp",
        "source/main/cpp/c_hash.cpp",
        "source/main/cpp/c_hash2.cpp",
        "source/main/cpp/c_hbb.cpp",
        "source/main/cpp/c_int128.cpp",
        "source/main/cpp/c_int256.cpp",
        "source/main/cpp/c_int64.cpp",
        "source/main/cpp/c_log.cpp",
        "source/main/cpp/c_log_to_console.cpp",
        "source/main/cpp/c_map.cpp",
        "source/main/cpp/c_memory.cpp",
        "source/main/cpp/c_printf.cpp",
        "source/main/cpp/c_qsort.cpp",
        "source/main/cpp/c_runes.cpp",
        "source/main/cpp/c_slice.cpp",
        "source/main/cpp/c_sscanf.cpp",
        "source/main/cpp/c_tree.cpp",
        "source/main/cpp/c_uint128.cpp",
        "source/main/cpp/c_uint256.cpp",
        "source/main/cpp/c_uint64.cpp",
        "source/main/cpp/c_va_list.cpp",
    }, &.{
        "-Wstrict-prototypes",
        "-Wwrite-strings",
        "-Wno-missing-field-initializers",
        "-Wno-shift-count-overflow",
        "-Wno-unused-parameter",
        "-DTARGET_RELEASE",
        "-DTARGET_TEST",
        "-DNDEBUG",
   });

    const cunittestlib = b.addStaticLibrary("cunittest", null);
    cunittestlib.setTarget(target);
    cunittestlib.setBuildMode(mode);
    cunittestlib.linkLibCpp();
    cunittestlib.addIncludeDir("../cunittest/source/main/include");
    cunittestlib.force_pic = true;
    cunittestlib.addCSourceFiles(&.{
        "../cunittest/source/main/cpp/entry/ut_Entry_Mac.cpp",
        "../cunittest/source/main/cpp/entry/ut_Entry_win32.cpp",
        "../cunittest/source/main/cpp/ut_AssertException.cpp",
        "../cunittest/source/main/cpp/ut_Checks.cpp",
        "../cunittest/source/main/cpp/ut_ReportAssert.cpp",
        "../cunittest/source/main/cpp/ut_Stdout_Mac.cpp",
        "../cunittest/source/main/cpp/ut_Stdout_Win32.cpp",
        "../cunittest/source/main/cpp/ut_StringBuilder.cpp",
        "../cunittest/source/main/cpp/ut_Test.cpp",
        "../cunittest/source/main/cpp/ut_TestList.cpp",
        "../cunittest/source/main/cpp/ut_TestReporter.cpp",
        "../cunittest/source/main/cpp/ut_TestReporterStdout.cpp",
        "../cunittest/source/main/cpp/ut_TestReporterTeamCity.cpp",
        "../cunittest/source/main/cpp/ut_TestResults.cpp",
        "../cunittest/source/main/cpp/ut_TestRunner.cpp",
        "../cunittest/source/main/cpp/ut_TestState.cpp",
        "../cunittest/source/main/cpp/ut_Test_Mac.cpp",
        "../cunittest/source/main/cpp/ut_Test_Win32.cpp",
        "../cunittest/source/main/cpp/ut_TimeConstraint.cpp",
        "../cunittest/source/main/cpp/ut_TimeHelpers_Mac.cpp",
        "../cunittest/source/main/cpp/ut_TimeHelpers_Win32.cpp",
        "../cunittest/source/main/cpp/ut_Utils.cpp",
        "../cunittest/source/main/cpp/ut_Utils_Mac.cpp",
        "../cunittest/source/main/cpp/ut_Utils_Win32.cpp",
    }, &.{
        "-Wstrict-prototypes",
        "-Wwrite-strings",
        "-Wno-missing-field-initializers",
        "-Wno-shift-count-overflow",
        "-Wno-unused-parameter",
        "-DTARGET_RELEASE",
        "-DTARGET_TEST",
        "-DNDEBUG",
   });
    if (cunittestlib.target.isWindows()) {    
        cunittestlib.defineCMacro("PLATFORM_PC", null);
    } else if (cunittestlib.target.isDarwin()) {
        cunittestlib.defineCMacro("PLATFORM_MAC", null);
    }

    const cbase_test = b.addExecutable("cbase-test", null);
    cbase_test.setTarget(target);
    cbase_test.setBuildMode(mode);
    cbase_test.install();
    cbase_test.linkLibCpp();
    cbase_test.linkLibrary(cbaselib);
    cbase_test.linkLibrary(cunittestlib);
    cbase_test.addIncludeDir("source/main/include");
    cbase_test.addIncludeDir("source/test/include");
    cbase_test.addIncludeDir("../cunittest/source/main/include");
    cbase_test.addCSourceFiles(&.{
        "../cunittest/source/main/cpp/entry/ut_Entry_Mac.cpp",
        "source/test/cpp/test_main.cpp",
        "source/test/cpp/test_span.cpp",
        "source/test/cpp/test_x_allocator.cpp",
        "source/test/cpp/test_x_binary_search.cpp",
        "source/test/cpp/test_x_bitfield.cpp",
        "source/test/cpp/test_x_btree.cpp",
        "source/test/cpp/test_x_buffer.cpp",
        "source/test/cpp/test_x_carray.cpp",
        "source/test/cpp/test_x_chars.cpp",
        "source/test/cpp/test_x_context.cpp",
        "source/test/cpp/test_x_darray.cpp",
        "source/test/cpp/test_x_double.cpp",
        "source/test/cpp/test_x_endian.cpp",
        "source/test/cpp/test_x_float.cpp",
        "source/test/cpp/test_x_guid.cpp",
        "source/test/cpp/test_x_hbb.cpp",
        "source/test/cpp/test_x_int64.cpp",
        "source/test/cpp/test_x_integer.cpp",
        "source/test/cpp/test_x_map_set.cpp",
        "source/test/cpp/test_x_memory.cpp",
        "source/test/cpp/test_x_qsort.cpp",
        "source/test/cpp/test_x_range.cpp",
        "source/test/cpp/test_x_runes_ascii.cpp",
        "source/test/cpp/test_x_runes_utf.cpp",
        "source/test/cpp/test_x_singleton.cpp",
        "source/test/cpp/test_x_slice.cpp",
        "source/test/cpp/test_x_sprintf.cpp",
        "source/test/cpp/test_x_sscanf.cpp",
        "source/test/cpp/test_x_tree.cpp",
        "source/test/cpp/test_x_triemap.cpp",
        "source/test/cpp/test_x_types.cpp",
        "source/test/cpp/test_x_va_list.cpp",
    }, &.{
        "-Wno-missing-field-initializers",
        "-Wno-shift-count-overflow",
        "-Wno-unused-parameter",
        "-DTARGET_RELEASE",
        "-DTARGET_TEST",
        "-DNDEBUG",
        "-DTARGET_MAC",
    });
    if (cbase_test.target.isWindows()) {    
        cbase_test.defineCMacro("PLATFORM_PC", null);
    } else if (cbase_test.target.isDarwin()) {
        cbase_test.defineCMacro("PLATFORM_MAC", null);
    }

}