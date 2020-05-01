{
  "targets": [
    {
      "target_name": "addon",
      "default_configuration": "Release",
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions", "-std=gnu++0x", "-std=gnu++1y"],
      "cflags_cc+": ["-std=c++17"],
      "sources": [ 
          "/simdjson/src/simdjson.cpp",
          "/src/addon.cpp" 
        ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      "xcode_settings": {
        "OTHER_CFLAGS": ["-O3", "-std=c++17", "-pthread"],
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
      },
      "msvs_settings": {
        "VCCLCompilerTool": {
          "AdditionalOptions": ["/std:c++17"]
        }
      }
    }
  ]
}
