project('xbase', 'c', 'cpp', default_options : ['c_std=c11', 'cpp_std=c++11'], version : '1.0.0', license : 'MIT')

add_global_arguments('-DMESON_TEST', language : 'cpp')
incdir = include_directories('include/')

sources = [ 'cpp/x_allocator.cpp', 
            'cpp/x_base.cpp', 
            'cpp/x_binary_search.cpp', 
            'cpp/x_console.cpp', 
            'cpp/x_debug.cpp', 
            'cpp/x_guid.cpp', 
            'cpp/x_int64.cpp', 
            'cpp/x_int128.cpp', 
            'cpp/x_int256.cpp', 
            'cpp/x_log_to_console.cpp', 
            'cpp/x_log.cpp', 
            'cpp/x_memory_std.cpp', 
            'cpp/x_qsort.cpp', 
            'cpp/x_slice.cpp', 
            'cpp/x_string_ascii.cpp', 
            'cpp/x_string_sprintf.cpp', 
            'cpp/x_string_sscanf.cpp', 
            'cpp/x_string_utf.cpp', 
            'cpp/x_tls.cpp', 
            'cpp/x_uint64.cpp', 
            'cpp/x_uint128.cpp', 
            'cpp/x_uint256.cpp', 
            'cpp/x_va_list.cpp', 
            ]

if host_machine.system() == 'windows'
    sources += [
        'cpp/x_allocator_system_win32.cpp',
        'cpp/x_console_win32.cpp',
    ]

    add_global_arguments('--DTARGET_PC_RELEASE', language : 'cpp')

endif

static_library('xbase', sources, include_directories: incdir, cpp_args : '-DTARGET_MACOS_RELEASE')