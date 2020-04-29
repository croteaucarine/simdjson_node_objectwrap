{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ 
          "/simdjson/src/simdjson.cpp",
          "/src/addon.cpp" 
        ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      "conditions": [
        ["OS=='mac'", {          
          "xcode_settings": {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'CLANG_CXX_LIBRARY': 'libc++',
            'MACOSX_DEPLOYMENT_TARGET': '10.7',
            "CLANG_X86_VECTOR_INSTRUCTIONS": "avx2",
            "OTHER_CFLAGS": ["-mavx2", "-mavx", "-mbmi", "-mpclmul", "-std=c++17", "-O3"],          
          }
        }],
        ["OS=='linux'", {          
          'cflags!': [ '-fno-exceptions' ],
          'cflags_cc!': [ '-fno-exceptions' ],
          'cflags_c': [ "-mavx2", "-mavx" ],
          "cflags_cc": [ "-mavx2", "-mavx", "mavx", "-mbmi", "-mpclmul", "-std=c++17", "-O3"],
        }],
        ["OS=='win'", {
          'msvs_settings': {
            'VCCLCompilerTool': { 'ExceptionHandling': 1 },
          }, 
          'cflags_c': [ "-mavx2", "-mavx" ],
          "cflags_cc": [ "-mavx2", "-mavx", "mavx", "-mbmi", "-mpclmul", "-std=c++17", "-O3"],
        }]
      ]
    }
  ]
}
