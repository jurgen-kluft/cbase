project('xbase-test', 'c', 'cpp', default_options : ['c_std=c11', 'cpp_std=c++11'], version : '1.0.0', license : 'MIT')

add_global_arguments('-DMESON_TEST', language : 'cpp')

incdir = include_directories('../main/include/', 'include/', '../../../xunittest/source/main/include/', 'include/')

sources = [ 'cpp/test_main.cpp', 
            'cpp/test_x_binary_search.cpp', 
            'cpp/test_x_bitfield.cpp', 
            'cpp/test_x_carray.cpp', 
            'cpp/test_x_double.cpp', 
            'cpp/test_x_endian.cpp', 
            'cpp/test_x_float.cpp', 
            'cpp/test_x_guid.cpp', 
            'cpp/test_x_int64.cpp', 
            'cpp/test_x_integer.cpp', 
            'cpp/test_x_memory_std.cpp', 
            'cpp/test_x_qsort.cpp', 
            'cpp/test_x_rbtree15.cpp', 
            'cpp/test_x_singleton.cpp', 
            'cpp/test_x_slice.cpp', 
            'cpp/test_x_string_sprintf.cpp', 
            'cpp/test_x_string_sscanf.cpp', 
            'cpp/test_x_string_std.cpp', 
            'cpp/test_x_string_utf.cpp', 
            'cpp/test_x_tls.cpp', 
            'cpp/test_x_tree_sentinel.cpp', 
            'cpp/test_x_tree.cpp', 
            'cpp/test_x_types.cpp', 
            'cpp/test_x_va_list.cpp', 
            ]

if host_machine.system() == 'windows'
    sources += [
        'cpp/x_allocator_system_win32.cpp',
        'cpp/x_console_win32.cpp',
    ]

    add_global_arguments('--DTARGET_PC_RELEASE', language : 'cpp')

endif

executable('xbase', sources, include_directories: incdir, cpp_args : '-DTARGET_MACOS_RELEASE')